#include "KeyBindingConfig.h"

sf::Keyboard::Key KeyBindingConfig::MoveLeft;
sf::Keyboard::Key KeyBindingConfig::MoveRight;
sf::Keyboard::Key KeyBindingConfig::MoveUp;
sf::Keyboard::Key KeyBindingConfig::MoveDown;

bool KeyBindingConfig::LoadConfigValues(std::vector<std::string>& configFileLines)
{
    return (ReadKey(configFileLines, MoveLeft, "Error decoding the move left key!") &&
        ReadKey(configFileLines, MoveRight, "Error reading in the move right key!") &&
        ReadKey(configFileLines, MoveUp, "Error reading in the move up key!") &&
        ReadKey(configFileLines, MoveDown, "Error reading in the move down key!"));
}

void KeyBindingConfig::WriteConfigValues()
{
	WriteKey("MoveLeft", MoveLeft);
	WriteKey("MoveRight", MoveRight);
	WriteKey("MoveUp", MoveUp);
	WriteKey("MoveDown", MoveDown);
}

KeyBindingConfig::KeyBindingConfig(const char* configName)
	: ConfigManager(configName)
{
}
