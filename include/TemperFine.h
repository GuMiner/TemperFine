#pragma once
#include <SFML\System.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <vector>
#include "ArmorConfig.h"
#include "Constants.h"
#include "FontManager.h"
#include "GraphicsConfig.h"
#include "ImageManager.h"
#include "KeyBindingConfig.h"
#include "MapManager.h"
#include "ModelManager.h"
#include "Physics.h"
#include "PhysicsConfig.h"
#include "Player.h"
#include "ShaderManager.h"
#include "Statistics.h"
#include "TurretConfig.h"
#include "Viewer.h"
#include "VoxelMap.h"

// Handles startup and the base graphics rendering loop.
class TemperFine
{
    // Configuration
    GraphicsConfig graphicsConfig;
    KeyBindingConfig keyBindingConfig;
    PhysicsConfig physicsConfig;

    // Managers
    sfg::SFGUI sfgui;
    FontManager fontManager;
    ImageManager imageManager;
    MapManager mapManager;
    ModelManager modelManager;
    ShaderManager shaderManager;

    // Game Data Configuration
    ArmorConfig armorConfig;
    TurretConfig turretConfig;

    // Game data
    Physics physics;
    Statistics statistics;
    Viewer viewer;
    VoxelMap voxelMap;
    vmath::mat4 perspectiveMatrix;

    std::vector<Player> players;
    sfg::Label::Ptr testLabel; // TODO remove when we work on GUI stuffs.
    MapInfo testMap; // TODO, there should be a vector of default maps, such as this one.

    // Non-graphics threads
    sf::Thread physicsThread;

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

