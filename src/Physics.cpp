#include <SFML\System.hpp>
#include "Logger.h"
#include "Physics.h"
#include "PhysicsConfig.h"
#include "Unit.h"

Physics::Physics()
{
    isAlive = true;
    isPaused = false;
}

void Physics::Initialize(std::vector<Player>* players, UnitRouter* unitRouter, Viewer* viewer, MapInfo* mapInfo)
{
    this->players = players;
    this->unitRouter = unitRouter;
    this->viewer = viewer;
    this->mapInfo = mapInfo;
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
                        testUnit.CreateNew(0, 0, turrets, vmath::vec3(step * i, step * j, 4.0f),
                           vmath::quaternion::fromAxisAngle(rotation, vmath::vec3(0.0f, 1.0f, 0.0f)) * vmath::quaternion::fromAxisAngle(vmath::radians(-90.0f), vmath::vec3(1.0f, 0.0f, 0.0f)));
                        (*players)[0].AddUnit(testUnit);
                        rotation += 0.20;
                    }
                }

                // TODO test routing code until click-selection is working.
                vmath::vec3i start = vmath::vec3i(0, 0, 0);
                vmath::vec3i end = vmath::vec3i(18, 18, 1);
                std::vector<vmath::vec3i> route;
                if (!mapSections.ComputeRoute(start, end, route))
                {
                    Logger::Log("Route computation failed!");
                }
                else
                {
                    std::vector<vmath::vec3i> betterRoute;
                    std::vector<vmath::vec3> graphicalRoute;
                    unitRouter->RefineRoute(mapSections.GetSubsections(), start, end, route, betterRoute, graphicalRoute);

                    Logger::Log("Updating player 0, unit 0, with graphical route with ", graphicalRoute.size(), " segments.");
                    (*players)[0].UpdateUnitRoute(0, graphicalRoute);
                }

                // Random valid route.

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
