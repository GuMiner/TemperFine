#include <string>
#include <sstream>
#include "Logger.h"
#include "StringUtils.h"
#include "ConversionUtils.h"
#include "ConfigManager.h"

ConfigManager::ConfigManager(const char* configFileName)
	: configFileName(configFileName)
{
}

bool ConfigManager::ReadBool(std::vector<std::string>& configFileLines, bool& boolean, const char* errorMessage)
{
	if (!ConversionUtils::LoadBool(configFileLines[++lineCounter], boolean))
	{
		Logger::Log(errorMessage);
		return false;
	}

    return true;
}

bool ConfigManager::ReadInt(std::vector<std::string>& configFileLines, int& integer, const char* errorMessage)
{
	if (!ConversionUtils::LoadInt(configFileLines[++lineCounter], integer))
	{
		Logger::Log(errorMessage);
		return false;
	}

    return true;
}
bool ConfigManager::ReadFloat(std::vector<std::string>& configFileLines, float& floatingPoint, const char* errorMessage)
{
	if (!ConversionUtils::LoadFloat(configFileLines[++lineCounter], floatingPoint))
	{
		Logger::Log(errorMessage);
		return false;
	}

    return true;
}
bool ConfigManager::ReadKey(std::vector<std::string>& configFileLines, sf::Keyboard::Key& key, const char* errorMessage)
{
	if (!ConversionUtils::LoadKey(configFileLines[++lineCounter], key))
	{
		Logger::Log(errorMessage);
		return false;
	}

    return true;
}

bool ConfigManager::ReadVector(std::vector<std::string>& configFileLines, vec::vec3& vector, const char* errorMessage)
{
	if (!ConversionUtils::LoadVector(configFileLines[++lineCounter], vector))
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

void ConfigManager::WriteVector(const char* itemName, vec::vec3& vector)
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

    if (!ConversionUtils::LoadInt(configFileLines[lineCounter], configVersion))
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
    std::vector<std::string> lines;
    if (!StringUtils::LoadConfigurationFile(configFileName, lines, commentLines))
    {
        Logger::Log("Unable to properly-parse the config-style file!");
        return false;
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
