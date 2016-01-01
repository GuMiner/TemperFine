#pragma once
#include "Constants.h"
#include "FontManager.h"
#include "GraphicsConfig.h"
#include "ShaderManager.h"
#include "Statistics.h"

// Handles startup and the base graphics rendering loop.
class TemperFine
{
    // Configuration
    GraphicsConfig graphicsConfig;

    // Managers
    ShaderManager shaderManager;
    FontManager fontManager;

    // Game data
    Statistics statistics;
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

    // Initializes generic OpenGL data after an OpenGL context is created.
    Constants::Status LoadGraphics();

    // Loads in assets after an OpenGL context is created.
    Constants::Status LoadAssets();

    // Runs the game loop.
    Constants::Status Run();

    // Deinitializes data that must be freed before program exit.
    void Deinitialize();
};

