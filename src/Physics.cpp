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

void Physics::Initialize(SyncBuffer* syncBuffer)
{
    this->syncBuffer = syncBuffer;
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

    vec::mat4 viewRotationMatrix = viewer.GetViewOrientation().asMatrix();
    vec::vec3 worldRay = PhysicsOps::ScreenRay(mousePos, screenSize, Constants::PerspectiveMatrix, viewRotationMatrix);
    
    // Check to see if we clicked a unit. You can only select your own units (player 0);
    Player& player = syncBuffer->LockPlayer(0);

    int collidedUnit = player.CollisionCheck(viewer.GetViewPosition(), worldRay);
    if (collidedUnit != -1)
    {
        player.ToggleUnitSelection(collidedUnit);
    }
    else
    {
        vec::vec3i hitVoxel;
        if (syncBuffer->HitByRay(mapSections, viewer.GetViewPosition(), worldRay, &hitVoxel))
        {
            syncBuffer->SetNewSelectedVoxel(hitVoxel);

            // If there are units selected, move them to the selected voxel (if possible)
            const std::set<int>& selectedUnits = player.GetSelectedUnits();
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
                    syncBuffer->RefineRoute(unitRouter, mapSections.GetSubsections(), start, hitVoxel, route, betterRoute, graphicalRoute);

                    Logger::Log("Updating player 0, selected unit ", selectedUnit, " with graphical route using ", graphicalRoute.size(), " segments.");
                    player.UpdateUnitRoute(selectedUnit, graphicalRoute);

                }
            }
        }
    }

    syncBuffer->UnlockPlayer(0);
}

void Physics::Run()
{
    sf::Clock clock;
    sf::Time physicsUpdateTime = sf::milliseconds(0);
    while (isAlive)
    {
        if (!isPaused)
        {
            // Update the viewer's position
            viewer.InputUpdate();

            // Synchronize with the GUI thread.
            syncBuffer->UpdateViewMatrix(viewer.GetViewPosition(), viewer.GetViewOrientation());
            syncBuffer->UpdateViewerPosition(viewer.GetViewPosition());

            if (isLeftMouseClicked)
            {
                HandleLeftMouseClicked();
                isLeftMouseClicked = false;
            }

            if (syncBuffer->UpdateRoundMapPhysics(mapSections))
            {
                // The round map was updated, so perform additional updates based on the map changing.
                Logger::Log("Round map physics updated!");

                // TODO test code, player shouldn't start with unit, and should be toggled off of something.

                // TODO test code, the player shouldn't start with units.
                std::vector<unsigned int> turrets;
                turrets.push_back(0); // TurretConfig::Turrets, first item.

                // The zeros are the indexes into ArmorConfig::Armors and BodyConfig::Bodies
                float step = 5.0f;
                unsigned int maxSize = 2;
                float rotation = 0.20f;
                for (unsigned int i = 0; i < maxSize; i++)
                {
                    for (unsigned int j = 0; j < maxSize; j++)
                    {
                        Unit unit = Unit(0, 0, turrets, vec::vec3(step * i, step * j, 4.0f),
                            vec::quaternion::fromAxisAngle(rotation, vec::vec3(0.0f, 1.0f, 0.0f)) * vec::quaternion::fromAxisAngle(MathOps::Radians(-90.0f), vec::vec3(1.0f, 0.0f, 0.0f)));
                        Player& player = syncBuffer->LockPlayer(0);
                        player.AddUnit(unit);
                        syncBuffer->UnlockPlayer(0);

                        rotation += 0.20f;
                    }
                }
            }

            // All units move
            syncBuffer->UpdatePlayers(physicsUpdateTime.asSeconds());
        }

        // The physics thread runs at a configurable delay, which we abide by here.
        physicsUpdateTime = clock.restart();
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
