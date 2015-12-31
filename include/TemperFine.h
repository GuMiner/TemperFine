#pragma once
#include "Constants.h"
#include "GraphicsConfig.h"

// Handles startup and the base graphics rendering loop.
class TemperFine
{
    // Configuration
    GraphicsConfig graphicsConfig;

    // Game data
    vmath::mat4 perspectiveMatrix;

    // Logs graphical settings so we have an idea of the OpenGL capabilities of the running machine.
    void LogGraphicsSettings();

    // Updates the camera perspective when the screen width / height has changed.
    void UpdatePerspective(unsigned int width, unsigned int height);
public:
    static Constants Constant;

    // Initializes TemperFine and any construction-time setup (such as threads).
    TemperFine();

    // Initializes data that can be setup before an OpenGL context is created.
    Constants::Status Initialize();

    // Initializes data after an OpenGL context is created.
    Constants::Status LoadGraphics();

    // Runs the game loop.
    Constants::Status Run();

    // Deinitializes data that must be freed before program exit.
    void Deinitialize();
};

