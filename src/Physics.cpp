#include <SFML\System.hpp>
#include "Physics.h"
#include "PhysicsConfig.h"

Physics::Physics()
{
    isAlive = true;
    isPaused = false;
}

void Physics::Initialize(Viewer* viewer, VoxelMap* voxelMap)
{
    this->viewer = viewer;
    this->voxelMap = voxelMap;
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

            // Updates statistics.
            viewer->viewPosition[2] = 2 + longRunningClock.getElapsedTime().asSeconds();

            // Updates the voxel map.
            voxelMap->Update();
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
