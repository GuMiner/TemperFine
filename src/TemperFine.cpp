#include <string>
#include <sstream>
#include <thread>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include "Logger.h"
#include "TemperFine.h"
#include "../version.h"

TemperFine::TemperFine()
    : graphicsConfig("config/graphics.txt"), keyBindingConfig("config/keyBindings.txt"), physicsConfig("config/physics.txt"),
      imageManager(), modelManager(&imageManager),
      armorConfig(&modelManager, "config/armors.txt"), bodyConfig(&modelManager, "config/bodies.txt"), turretConfig(&modelManager, "config/turrets.txt"),
      physics(), physicsThread(&Physics::Run, &physics)
{
}

void TemperFine::LogGraphicsSettings()
{
    std::stringstream graphicsSettings;
    graphicsSettings << "OpenGL vendor: " << glGetString(GL_VENDOR) << ", version " << glGetString(GL_VERSION) << ", renderer " << glGetString(GL_RENDERER);
    Logger::Log(graphicsSettings.str().c_str());

	graphicsSettings.str("");
	graphicsSettings << "OpenGL extensions: " << glGetString(GL_EXTENSIONS);
	Logger::Log(graphicsSettings.str().c_str());

	GLint maxTextureUnits, maxUniformBlockSize;
	GLint maxVertexUniformBlocks, maxFragmentUniformBlocks;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &maxTextureUnits);
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &maxVertexUniformBlocks);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &maxFragmentUniformBlocks);
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);

	graphicsSettings.str("");
	graphicsSettings << "Max Texture Units: " << maxTextureUnits << ", Max Uniform Size: " << (maxUniformBlockSize/1024) << " kB";
	Logger::Log(graphicsSettings.str().c_str());

	graphicsSettings.str("");
	graphicsSettings << "Max Vertex Uniform Blocks: " << maxVertexUniformBlocks << ", Max Fragment Uniform Blocks: " << maxFragmentUniformBlocks;
	Logger::Log(graphicsSettings.str().c_str());
}

void TemperFine::UpdatePerspective(unsigned int width, unsigned int height)
{
    // Letterboxing is done at the top and bottom.
    float necessaryWidth = (float)height * Constants::ASPECT;
    if (necessaryWidth > width)
    {
        // Letterbox the top and the bottom of the screen so that the aspect ratio is met
        float effectiveHeight = (float)width / Constants::ASPECT;
        float heightDelta = ((float)height - effectiveHeight) / 2.0f;
        glViewport(0, (int)heightDelta, (GLsizei)width, (GLsizei)effectiveHeight);
    }
    else
    {
        // Letterbox the left and the right so that the aspect ratio is met.
        float widthDelta = ((float)width - necessaryWidth) / 2.0f;
        glViewport((GLint)widthDelta, (GLint)0, (GLsizei)necessaryWidth, (GLsizei)height);
    }
}

// Performs initialization that can be done without a GPU context.
Constants::Status TemperFine::Initialize()
{
    Logger::Log("Loading graphics config file...");
    if (!graphicsConfig.ReadConfiguration())
    {
		Logger::Log("Bad graphics config file!");
        return Constants::Status::BAD_CONFIG;
    }

    Logger::Log("Loading key binding config file...");
    if (!keyBindingConfig.ReadConfiguration())
    {
        Logger::Log("Bad key binding config file!");
        return Constants::Status::BAD_CONFIG;
    }

    Logger::Log("Loading physics config file...");
    if (!physicsConfig.ReadConfiguration())
    {
        Logger::Log("Bad physics config file!");
        return Constants::Status::BAD_CONFIG;
    }

    Logger::Log("Loading maps...");
    if (!mapManager.ReadMap("maps/test.txt", testMap))
    {
        Logger::Log("Bad test map!");
        return Constants::Status::BAD_MAP;
    }

    Logger::Log("Configuration loaded!");
    return Constants::Status::OK;
}

Constants::Status TemperFine::LoadGraphics()
{
    // Setup GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        Logger::LogErrorCode("GLEW startup failure", err);
        return Constants::Status::BAD_GLEW;
    }

    // Log graphics information for future reference
    LogGraphicsSettings();

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable line and polygon smoothing
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    // Multisample if available
    glEnable(GL_MULTISAMPLE);

    // Let OpenGL shaders determine point sizes.
	glEnable(GL_PROGRAM_POINT_SIZE);

    // Disable face culling so that see-through flat objects work.
    glDisable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    // Cutout faces that are hidden by other faces.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Perspective display
    perspectiveMatrix = vmath::perspective(Constants::FOV_Y, Constants::ASPECT, Constants::NEAR_PLANE, Constants::FAR_PLANE);

    // Assets
    return LoadAssets();
}

Constants::Status TemperFine::LoadAssets()
{
    // Game Data configuration files.
    Logger::Log("Loading armor config file...");
    if (!armorConfig.ReadConfiguration())
    {
        Logger::Log("Bad armor config file!");
        return Constants::Status::BAD_CONFIG;
    }

    Logger::Log("Loading body config file...");
    if (!bodyConfig.ReadConfiguration())
    {
        Logger::Log("Bad body config file!");
        return Constants::Status::BAD_CONFIG;
    }

    Logger::Log("Loading turret config file...");
    if (!turretConfig.ReadConfiguration())
    {
        Logger::Log("Bad turret config file!");
        return Constants::Status::BAD_CONFIG;
    }

    // Fonts
    Logger::Log("Font loading...");
    if (!fontManager.LoadFont(&shaderManager, "fonts/DejaVuSans.ttf"))
    {
        return Constants::Status::BAD_FONT;
    }

    Logger::Log("Font loading done!");

    // Statistics
    Logger::Log("Statistics loading...");
    if (!statistics.Initialize(&fontManager))
    {
        return Constants::Status::BAD_STATS;
    }

    // Voxel Map
    Logger::Log("Voxel map...");
    if (!voxelMap.Initialize(imageManager, shaderManager))
    {
        return Constants::Status::BAD_VOXEL_MAP;
    }

    // TODO: This should be setup from some sort of menu / user input / network input
    voxelMap.SetupFromMap(&testMap);

    // TODO Test setup code remove.
    testLabel = sfg::Label::Create("Temper Fine Test Label.");

    // Load the current player, who is always the first element in the players list.
    Player currentPlayer;
    players.push_back(currentPlayer);

    // Now that *all* the models have loaded, prepare for rendering models.
    if (!modelManager.InitializeOpenGlResources(shaderManager))
    {
        return Constants::Status::BAD_SHADERS;
    }

    modelManager.ResetOpenGlModelData();

    // Physics
    Logger::Log("Physics loading...");
    physics.Initialize(&viewer, &voxelMap);

    physicsThread.launch();
    Logger::Log("Physics Thread Started!");

    return Constants::Status::OK;
}

Constants::Status TemperFine::Run()
{
    // 24 depth bits, 8 stencil bits, 8x AA, major version 4.
    Logger::Log("Graphics Initializing...");
    sf::ContextSettings contextSettings = sf::ContextSettings(24, 8, 8, 4, 0);

    sf::Uint32 style = GraphicsConfig::IsFullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close;
    sf::RenderWindow window(sf::VideoMode(GraphicsConfig::ScreenWidth, GraphicsConfig::ScreenHeight), "Temper Fine", style, contextSettings);

    // Now that we have an OpenGL Context, load our graphics.
    Constants::Status firstTimeSetup = LoadGraphics();
    if (firstTimeSetup != Constants::Status::OK)
    {
        return firstTimeSetup;
    }

    UpdatePerspective(window.getSize().x, window.getSize().y);
    Logger::Log("Graphics Initialized!");

    // TODO move to a menu class

    sf::Image techImage;
    // TODO error checking
    techImage.loadFromFile("images/menu/techs.png");

    sfg::Button::Ptr techsButton = sfg::Button::Create("Techs Button");
    sfg::Image::Ptr techImageSfg = sfg::Image::Create(techImage);
    techsButton->SetImage(techImageSfg);

    // Setup UI specific stuffs TODO remove testing code here
    sfg::Box::Ptr testBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.0f);
    testBox->Pack(testLabel);
    testBox->Pack(techsButton);

    sfg::Window::Ptr uiWindow = sfg::Window::Create();
    uiWindow->SetTitle("Temper Fine Test Window!");
    uiWindow->Add(testBox);

    sfg::Desktop desktop;
    desktop.Add(uiWindow);

    sf::Clock clock;
    sf::Clock frameClock;
    sf::Time clockStartTime;
    bool alive = true;
    bool paused = false;
    while (alive)
    {
        clockStartTime = clock.getElapsedTime();

        // Handle all events.
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Update SF GUI
            desktop.HandleEvent(event);

            if (event.type == sf::Event::Closed)
            {
                alive = false;
            }
            else if (event.type == sf::Event::LostFocus)
            {
                paused = true;
                physics.Pause();
                // musicManager.Pause();
            }
            else if (event.type == sf::Event::GainedFocus)
            {
                paused = false;
                physics.Resume();
                // musicManager.Resume();
            }
            else if (event.type == sf::Event::Resized)
            {
                UpdatePerspective(event.size.width, event.size.height);
            }
        }

        // TODO only update if the view position has updated.
        statistics.UpdateStats(viewer.viewPosition);

        vmath::mat4 lookAtMatrix = viewer.viewOrientation.asMatrix() * vmath::translate(-viewer.viewPosition);
        vmath::mat4 projectionMatrix = perspectiveMatrix * lookAtMatrix;

        // Render, only if non-paused.
        if (!paused)
        {
            // Clear the screen (and depth buffer) before any rendering begins.
            const GLfloat color[] = { 0, 0, 0, 1 };
            const GLfloat one = 1.0f;
            glClearBufferfv(GL_COLOR, 0, color);
            glClearBufferfv(GL_DEPTH, 0, &one);

            // TODO test code remove, renders a model using the model manager.
            vmath::mat4 identityMatrix = vmath::mat4::identity();
            modelManager.RenderModel(projectionMatrix, ArmorConfig::Armors[0].armorModelId, identityMatrix);

            vmath::mat4 translationMatrix = vmath::translate(200, 0, 0);
            modelManager.RenderModel(projectionMatrix, ArmorConfig::Armors[0].armorModelId, translationMatrix);

            translationMatrix = vmath::translate(100, 0, 0) * vmath::scale(0.1f, 0.1f, 0.1f);
            modelManager.RenderModel(projectionMatrix, TurretConfig::Turrets[0].turretModelId, translationMatrix);

            translationMatrix = vmath::translate(-50, 20, 0) * translationMatrix;
            modelManager.RenderModel(projectionMatrix, BodyConfig::Bodies[0].bodyModelId, translationMatrix);

            // Renders the voxel map
            voxelMap.Render(projectionMatrix);

            // Renders the statistics. Note that this just takes the perspective matrix, not accounting for the viewer position.
            statistics.RenderStats(perspectiveMatrix);

            // Renders the UI. Note that we unbind the current vertex array to avoid MyGUI messing with our data.
            // window.resetGLStates(); // This crashes on my AMD card.
            desktop.Update(frameClock.restart().asSeconds());
            sfgui.Display(window);

            window.display();
        }

        // Delay to run approximately at our maximum framerate.
        sf::Int64 sleepDelay = (1000000 / Constants::MAX_FRAMERATE) - clock.getElapsedTime().asMicroseconds() - clockStartTime.asMicroseconds();
        if (sleepDelay > 0)
        {
            sf::sleep(sf::microseconds(sleepDelay));
        }
    }

    return Constants::Status::OK;
}

void TemperFine::Deinitialize()
{
    Logger::Log("Music Thread Stopping...");
    //musicManager.Stop();

    Logger::Log("Physica Thread Stopping...");
    physics.Stop();
    physicsThread.wait();
    Logger::Log("Physics Thread Stopped.");

   // musicThread.wait();
    Logger::Log("Music Thread Stopped.");
}

// Runs the main application.
int main(int argc, char* argv[])
{
    // Startup 'static' stuff
    TemperFine::Constant = Constants();

    Logger::Setup();

    std::stringstream startupDetails;
    startupDetails << "TemperFine " << AutoVersion::MAJOR << "." << AutoVersion::MINOR << std::endl;
    Logger::Log(startupDetails.str().c_str());

    Constants::Status runStatus;
    std::unique_ptr<TemperFine> temperFine(new TemperFine());

    // Run the application.
    runStatus = temperFine->Initialize();
    if (runStatus == Constants::Status::OK)
    {
        runStatus = temperFine->Run();
        temperFine->Deinitialize();
    }
    else
    {
        Logger::LogError("Could not initialize Temper Fine!");
    }

    // Wait before closing for display purposes.
    Logger::Log("Application End!");
    Logger::Shutdown();
    sf::sleep(sf::milliseconds(1000));

    // Log is auto-shutdown.
    return (int)runStatus;
}
