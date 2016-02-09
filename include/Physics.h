#pragma once
#include "Statistics.h"
#include "Viewer.h"
#include "VoxelMap.h"
#include "MapSections.h"

class Physics
{
    public:
        Physics();

        // Initializes the physics thread with variables it needs.
        void Initialize(Viewer* viewer, MapInfo* mapInfo);

        // Runs actions on the physics thread.
        void Run();

        // Pauses the physics thread updates.
        void Pause();

        // Resumes the physics thread updates.
        void Resume();

        // Stops the physics thread updates.
        void Stop();

    private:
        Viewer* viewer;

        MapInfo* mapInfo;

        MapSections mapSections;

        bool isAlive;
        bool isPaused;
};
