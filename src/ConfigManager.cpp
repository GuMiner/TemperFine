#include <string>
#include <sstream>
#include "Logger.h"
#include "StringUtils.h"
#include "ConfigManager.h"

ConfigManager::ConfigManager(const char* configFileName)
	: CommentString("#"), configFileName(configFileName)
{
}

// Loads in a boolean configuration value.
bool ConfigManager::LoadBool(bool& boolean)
{
    std::string tempInput;
    return !(!StringUtils::SplitAndGrabSecondary(currentLine, tempInput) || !StringUtils::ParseBoolFromString(tempInput, boolean));
}

// Loads in an integer configuration value.
bool ConfigManager::LoadInt(int& integer)
{
    std::string tempInput;
    return !(!StringUtils::SplitAndGrabSecondary(currentLine, tempInput) || !StringUtils::ParseIntFromString(tempInput, integer));
}

// Loads in a floating-point configuration value.
bool ConfigManager::LoadFloat(float& floatingPoint)
{
    std::string tempInput;
    return !(!StringUtils::SplitAndGrabSecondary(currentLine, tempInput) || !StringUtils::ParseFloatFromString(tempInput, floatingPoint));
}

// Loads in an SFML keyboard key.
bool ConfigManager::LoadKey(sf::Keyboard::Key& key)
{
    int keyInt;
    if (!LoadInt(keyInt))
    {
        return false;
    }

    key = (sf::Keyboard::Key)keyInt;
    return true;
}

// Loads in a 3-valued floating point vector.
bool ConfigManager::LoadVector(vmath::vec3& vector)
{
	std::vector<std::string> stringParts;
	StringUtils::Split(currentLine, StringUtils::Space, true, stringParts);

	if (stringParts.size() != 4)
	{
		return false;
	}

	if (!StringUtils::ParseFloatFromString(stringParts[1], vector[0]) ||
		!StringUtils::ParseFloatFromString(stringParts[2], vector[1]) ||
		!StringUtils::ParseFloatFromString(stringParts[3], vector[2]))
	{
		return false;
	}

	return true;
}

bool ConfigManager::ReadBool(std::vector<std::string>& configFileLines, bool& boolean, const char* errorMessage)
{
    currentLine = configFileLines[++lineCounter];
	if (!LoadBool(boolean))
	{
		Logger::Log(errorMessage);
		return false;
	}

    return true;
}

bool ConfigManager::ReadInt(std::vector<std::string>& configFileLines, int& integer, const char* errorMessage)
{
    currentLine = configFileLines[++lineCounter];
	if (!LoadInt(integer))
	{
		Logger::Log(errorMessage);
		return false;
	}

    return true;
}
bool ConfigManager::ReadFloat(std::vector<std::string>& configFileLines, float& floatingPoint, const char* errorMessage)
{
    currentLine = configFileLines[++lineCounter];
	if (!LoadFloat(floatingPoint))
	{
		Logger::Log(errorMessage);
		return false;
	}

    return true;
}
bool ConfigManager::ReadKey(std::vector<std::string>& configFileLines, sf::Keyboard::Key& key, const char* errorMessage)
{
    currentLine = configFileLines[++lineCounter];
	if (!LoadKey(key))
	{
		Logger::Log(errorMessage);
		return false;
	}

    return true;
}

bool ConfigManager::ReadVector(std::vector<std::string>& configFileLines, vmath::vec3& vector, const char* errorMessage)
{
    currentLine = configFileLines[++lineCounter];
	if (!LoadVector(vector))
	{
		Logger::Log(errorMessage);
		return false;
	}

    return true;
}


void ConfigManager::WriteBool(const char* itemName, bool& boolean)
{
    std::stringstream tempOutput;
    tempOutput << itemName << StringUtils::Space << std::boolalpha << boolean;
    outputLines.push_back(tempOutput.str());
}

void ConfigManager::WriteInt(const char* itemName, int& integer)
{
    std::stringstream tempOutput;
    tempOutput << itemName << StringUtils::Space << integer;
    outputLines.push_back(tempOutput.str());
}

void ConfigManager::WriteFloat(const char* itemName, float& floatingPoint)
{
    double tempValue = floatingPoint;
    std::stringstream tempOutput;
    tempOutput << itemName << StringUtils::Space << tempValue;
    outputLines.push_back(tempOutput.str());
}

void ConfigManager::WriteKey(const char* itemName, sf::Keyboard::Key& key)
{
    std::stringstream tempOutput;
    tempOutput << itemName << StringUtils::Space << (int)key;
    outputLines.push_back(tempOutput.str());
}

void ConfigManager::WriteVector(const char* itemName, vmath::vec3& vector)
{
	double x = vector[0];
	double y = vector[1];
	double z = vector[2];
	std::stringstream tempOutput;
	tempOutput << itemName << StringUtils::Space << x << StringUtils::Space << y << StringUtils::Space << z;
	outputLines.push_back(tempOutput.str());

}

// Loads in all the configuration values.
bool ConfigManager::LoadConfigurationValues(std::vector<std::string>& configFileLines)
{
    lineCounter = 0;

	currentLine = configFileLines[lineCounter];
    if (!LoadInt(configVersion))
    {
        Logger::Log("Error decoding the configuration file version!");
        return false;
    }

	return LoadConfigValues(configFileLines);
}

void ConfigManager::WriteConfigurationValues()
{
    WriteInt("ConfigVersion", configVersion);
	WriteConfigValues();
}

// Reads in the configuration and sets up the variables listed
bool ConfigManager::ReadConfiguration()
{
    // Load, split, and remove comment lines from the configuration file.
    std::string entireFile;
    std::vector<std::string> lines;
    if (!StringUtils::LoadStringFromFile(configFileName, entireFile))
    {
        Logger::Log("Unable to load the config file!");
        return false;
    }

    int currentLine = 0;
    StringUtils::Split(entireFile, StringUtils::Newline, false, lines);
    for (unsigned int i = 0; i < lines.size(); i++)
    {
        if (StringUtils::StartsWith(lines[i], CommentString))
        {
            commentLines[currentLine] = std::string(lines[i]);

            lines.erase(lines.begin() + i);
            i--;
        }
        else if (StringUtils::IsWhitespaceOrEmpty(lines[i]))
        {
            commentLines[currentLine] = std::string(lines[i]);

            lines.erase(lines.begin() + i);
            i--;
        }

        ++currentLine;
    }

    // Parse out the configuration values from the file.
    if (!LoadConfigurationValues(lines))
    {
        Logger::Log("Invalid values were found in the config file!");
        return false;
    }

    return true;
}

// Writes out any changes to the static variables to the configuration.
bool ConfigManager::WriteConfiguration()
{
    // Write out our config file, ensuring we re-insert comment lines as appropriate.
	outputLines.clear();
    WriteConfigurationValues();

    for (unsigned int i = 0; i < outputLines.size(); i++)
    {
        if (commentLines.count(i) != 0)
        {
            // There's a comment line at this position, so add it in.
			outputLines.insert(outputLines.begin() + i, commentLines[i]);
        }
    }

    std::stringstream resultingFile;
    for (unsigned int i = 0; i < outputLines.size(); i++)
    {
        resultingFile << outputLines[i] << StringUtils::Newline;
    }

    std::string resultingFileString = resultingFile.str();
    std::string configFileNameString = std::string(configFileName);
    if (!StringUtils::WriteStringToFile(configFileNameString, resultingFileString))
    {
        Logger::Log("Couldn't write out the updated config file!");
        return false;
    }

    return true;
}
