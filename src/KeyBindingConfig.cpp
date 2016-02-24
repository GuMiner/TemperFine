#include "KeyBindingConfig.h"

sf::Keyboard::Key KeyBindingConfig::MoveLeft;
sf::Keyboard::Key KeyBindingConfig::MoveRight;
sf::Keyboard::Key KeyBindingConfig::MoveUp;
sf::Keyboard::Key KeyBindingConfig::MoveDown;

sf::Keyboard::Key KeyBindingConfig::ToggleTechProgressWindow;
sf::Keyboard::Key KeyBindingConfig::ToggleTechTreeWindow;

bool KeyBindingConfig::LoadConfigValues(std::vector<std::string>& configFileLines)
{
    return (ReadKey(configFileLines, MoveLeft, "Error decoding the move left key!") &&
        ReadKey(configFileLines, MoveRight, "Error reading in the move right key!") &&
        ReadKey(configFileLines, MoveUp, "Error reading in the move up key!") &&
        ReadKey(configFileLines, MoveDown, "Error reading in the move down key!") &&
        ReadKey(configFileLines, ToggleTechProgressWindow, "Error reading in the tech progress key!") &&
        ReadKey(configFileLines, ToggleTechTreeWindow, "Error reading in the tech tree key!"));
}

void KeyBindingConfig::WriteConfigValues()
{
    WriteKey("MoveLeft", MoveLeft);
    WriteKey("MoveRight", MoveRight);
    WriteKey("MoveUp", MoveUp);
    WriteKey("MoveDown", MoveDown);
    WriteKey("ToggleTechProgressWindow", ToggleTechProgressWindow);
    WriteKey("ToggleTechTreeWindow", ToggleTechTreeWindow);
}

KeyBindingConfig::KeyBindingConfig(const char* configName)
    : ConfigManager(configName)
{
}
