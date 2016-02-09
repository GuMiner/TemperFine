#pragma once
#include <SFML\System.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <vector>
#include "ArmorConfig.h"
#include "BodyConfig.h"
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
#include "Scenery.h"
#include "ShaderManager.h"
#include "Statistics.h"
#include "TechConfig.h"
#include "TechTreeWindow.h"
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

    // GUI
    TechTreeWindow techTreeWindow;

    // Game Data Configuration
    TechConfig techConfig;
    ArmorConfig armorConfig;
    BodyConfig bodyConfig;
    TurretConfig turretConfig;

    // Game data
    Physics physics;
    Statistics statistics;
    Viewer viewer;
    VoxelMap voxelMap;
    Scenery scenery;
    vmath::mat4 perspectiveMatrix;

    std::vector<Player> players;
    MapInfo testMap; // TODO, there should be a vector of default maps, such as this one.

    // Non-graphics threads
    sf::Thread physicsThread;

    // Logs graphical settings so we have an idea of the OpenGL capabilities of the running machine.
    void LogGraphicsSettings();

    // Updates the camera perspective when the screen width / height has changed.
    void UpdatePerspective(unsigned int width, unsigned int height);

    // Performs updates that must be performed on the OpenGL thread. Needs to coordinate with the physics thread.
    void PerformGuiThreadUpdates(float currentGameTime);

    // Handles GUI-based events, such as closing the application, resizing the window, etc.
    void HandleEvents(sfg::Desktop& desktop, sf::RenderWindow& window, bool& alive, bool& paused);

    // Renders the scene.
    void Render(sfg::Desktop& desktop, sf::RenderWindow& window, sf::Clock& guiClock, vmath::mat4& viewMatrix);

public:
    static Constants Constant;

    // Initializes TemperFine and any construction-time setup (such as threads).
    TemperFine();

    // Initializes data that can be setup before an OpenGL context is created.
    Constants::Status Initialize();

    // Initializes generic OpenGL data after an OpenGL context is created.
    Constants::Status LoadGraphics(sfg::Desktop* desktop);

    // Loads in assets after an OpenGL context is created.
    Constants::Status LoadAssets(sfg::Desktop* desktop);

    // Runs the game loop.
    Constants::Status Run();

    // Deinitializes data that must be freed before program exit.
    void Deinitialize();
};

