#include <set>
#include <SFML\System.hpp>
#include "Constants.h"
#include "Logger.h"
#include "MathOps.h"
#include "PhysicsOps.h"
#include "PhysicsConfig.h"
#include "Unit.h"
#include "Physics.h"

Physics::Physics()
{
    isAlive = true;
    isPaused = false;

    isLeftMouseClicked = true;
}

void Physics::Initialize(ModelManager* modelManager, std::vector<Player>* players, UnitRouter* unitRouter, Viewer* viewer, VoxelMap* voxelMap, MapInfo* mapInfo)
{
    this->modelManager = modelManager;
    this->players = players;
    this->unitRouter = unitRouter;
    this->viewer = viewer;
    this->voxelMap = voxelMap;
    this->mapInfo = mapInfo;
}

// Queues a mouse click for manipulation with the physics thread.
void Physics::QueueLeftMouseClick(int x, int y, int xSize, int ySize)
{
    leftMouseClickData.x = x;
    leftMouseClickData.y = y;
    leftMouseClickData.xSize = xSize;
    leftMouseClickData.ySize = ySize;
    isLeftMouseClicked = true;
}

// Handles left mouse clicks from the physics thread.
void Physics::HandleLeftMouseClicked()
{
    vec::vec2 mousePos = vec::vec2((float)leftMouseClickData.x, (float)leftMouseClickData.y);
    vec::vec2 screenSize = vec::vec2((float)leftMouseClickData.xSize, (float)leftMouseClickData.ySize);

    vec::mat4 viewRotationMatrix = viewer->viewOrientation.asMatrix();
    vec::vec3 worldRay = PhysicsOps::ScreenRay(mousePos, screenSize, Constants::PerspectiveMatrix, viewRotationMatrix);

    // Check to see if we clicked a unit. You can only select your own units.
    int collidedUnit = (*players)[0].CollisionCheck(*modelManager, viewer->viewPosition, worldRay);
    if (collidedUnit != -1)
    {
        (*players)[0].ToggleUnitSelection(collidedUnit);
    }
    else
    {
        vec::vec3i hitVoxel;
        if (mapSections.HitByRay(mapInfo, viewer->viewPosition, worldRay, &hitVoxel))
        {
            voxelMap->SetSelectedVoxel(hitVoxel);

            // If there are units selected, move them to the selected voxel (if possible)
            const std::set<int>& selectedUnits = (*players)[0].GetSelectedUnits();
            for (int selectedUnit : selectedUnits)
            {
                vec::vec3i start = vec::vec3i(0, 0, 0); // TODO invalid, used for testing purposes. until units have predefined current voxels.

                std::vector<vec::vec3i> route;
                if (!mapSections.ComputeRoute(start, hitVoxel, route))
                {
                    Logger::Log("Route computation failed from ", start.x, ", ", start.y, ", ", start.z, " to ", hitVoxel.x, ", ", hitVoxel.y, ", ", hitVoxel.z, ".");
                }
                else
                {
                    // Send the route to the selected unit.
                    std::vector<vec::vec3i> betterRoute;
                    std::vector<vec::vec3> graphicalRoute;
                    Logger::Log("Refining route...");
                    unitRouter->RefineRoute(mapInfo, mapSections.GetSubsections(), start, hitVoxel, route, betterRoute, graphicalRoute);

                    Logger::Log("Updating player 0, selected unit ", selectedUnit, " with graphical route using ", graphicalRoute.size(), " segments.");
                    (*players)[0].UpdateUnitRoute(selectedUnit, graphicalRoute);
                }
            }
        }
    }
}

void Physics::Run()
{
    sf::Clock clock;
    sf::Clock longRunningClock;
    while (isAlive)
    {
        if (!isPaused)
        {
			// Update the viewer's position
            viewer->InputUpdate();

            if (isLeftMouseClicked)
            {
                HandleLeftMouseClicked();
                isLeftMouseClicked = false;
            }

            if (mapSections.NeedsRecomputation())
            {
                mapSections.RecomputeMapSections(mapInfo);

                // TODO test code, player shouldn't start with unit, and should be toggled off of something.

                // TODO test code, the player shouldn't start with units.
                Unit testUnit;
                std::vector<unsigned int> turrets;
                turrets.push_back(0); // TurretConfig::Turrets, first item.

                // The zeros are the indexes into ArmorConfig::Armors and BodyConfig::Bodies
                float step = 5.0f;
                unsigned int maxSize = 2;
                float rotation = 0.20;
                for (unsigned int i = 0; i < maxSize; i++)
                {
                    for (unsigned int j = 0; j < maxSize; j++)
                    {
                        testUnit.CreateNew(0, 0, turrets, vec::vec3(step * i, step * j, 4.0f),
                           vec::quaternion::fromAxisAngle(rotation, vec::vec3(0.0f, 1.0f, 0.0f)) * vec::quaternion::fromAxisAngle(MathOps::Radians(-90.0f), vec::vec3(1.0f, 0.0f, 0.0f)));
                        (*players)[0].AddUnit(testUnit);
                        rotation += 0.20;
                    }
                }
            }

            // All units move
            for (Player& player : (*players))
            {
                player.MoveUnits();
            }
        }

        // The physics thread runs at a configurable delay, which we abide by here.
        sf::Time physicsUpdateTime = clock.restart();
        int sleepAmount = PhysicsConfig::PhysicsThreadDelay - physicsUpdateTime.asMilliseconds();
        if (sleepAmount > 0)
        {
            sf::sleep(sf::milliseconds(sleepAmount));
        }
    }
}

void Physics::Pause()
{
    isPaused = true;
}

void Physics::Resume()
{
    isPaused = false;
}

void Physics::Stop()
{
    isAlive = false;
}
