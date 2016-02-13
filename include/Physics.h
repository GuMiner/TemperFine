#pragma once
#include <vector>
#include "MapSections.h"
#include "ModelManager.h"
#include "Player.h"
#include "UnitRouter.h"
#include "Viewer.h"
#include "VoxelMap.h"

// Simple structure for where the mouse was clicked and the screen size at the time of clicking.
struct MouseClickData
{
    int x;
    int y;
    int xSize;
    int ySize;
};

// Manages physics.
// Physics encompasses everything compuation-wise that should run on a separate thread to avoid frame lag.
// Usually, events get sent from graphics (TemperFine) to physics, are processed here, and then updated remotely.
class Physics
{
    public:
        Physics();

        // Initializes the physics thread with variables it needs.
        void Initialize(ModelManager* modelManager, std::vector<Player>* players, UnitRouter* unitRouter, Viewer* viewer, VoxelMap* voxelMap, MapInfo* mapInfo);

        // Queues a mouse click for future processing.
        void QueueLeftMouseClick(int x, int y, int xSize, int ySize);

        // Runs actions on the physics thread.
        void Run();

        // Pauses the physics thread updates.
        void Pause();

        // Resumes the physics thread updates.
        void Resume();

        // Stops the physics thread updates.
        void Stop();

    private:
        ModelManager* modelManager;
        std::vector<Player>* players;
        UnitRouter* unitRouter;
        Viewer* viewer;
        VoxelMap* voxelMap;
        MapInfo* mapInfo;

        MapSections mapSections;

        bool isAlive;
        bool isPaused;

        bool isLeftMouseClicked;
        MouseClickData leftMouseClickData;
        void HandleLeftMouseClicked();
};
