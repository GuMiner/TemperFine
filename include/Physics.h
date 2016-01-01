#pragma once
#include "Statistics.h"
#include "Viewer.h"

class Physics
{
    Viewer* viewer;
    public:
        Physics();

        // Initializes the physics thread with variables it needs.
        void Initialize(Viewer* viewer);

        // Runs actions on the physics thread.
        void Run();

        // Pauses the physics thread updates.
        void Pause();

        // Resumes the physics thread updates.
        void Resume();

        // Stops the physics thread updates.
        void Stop();
    protected:
    private:
        bool isAlive;
        bool isPaused;
};
