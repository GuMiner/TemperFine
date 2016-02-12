#pragma once
#include <map>
#include <string>
#include <vector>
#include <SFML\Window.hpp>
#include "Vec.h"

// Loads in the configuration details for the rest of the system.
class ConfigManager
{
private:
	// Empty lines are counted as comment lines to preserve formatting of spacing.
	std::map<int, std::string> commentLines;

    const char* configFileName;
protected:
	int lineCounter;

    bool ReadBool(std::vector<std::string>& configFileLines, bool& boolean, const char* errorMessage);
    bool ReadInt(std::vector<std::string>& configFileLines, int& integer, const char* errorMessage);
    bool ReadFloat(std::vector<std::string>& configFileLines, float& floatingPoint, const char* errorMessage);
    bool ReadKey(std::vector<std::string>& configFileLines, sf::Keyboard::Key& key, const char* errorMessage);
	bool ReadVector(std::vector<std::string>& configFileLines, vec::vec3& vector, const char* errorMessage);

	std::vector<std::string> outputLines;
    void WriteBool(const char* itemName, bool& boolean);
    void WriteInt(const char* itemName, int& integer);
    void WriteFloat(const char* itemName, float& floatingPoint);
    void WriteKey(const char* itemName, sf::Keyboard::Key& key);
	void WriteVector(const char* itemName, vec::vec3& vector);

	// Performs config-manager specific loading and writing. Called by read/write configuration.
    bool LoadConfigurationValues(std::vector<std::string>& lines);
    void WriteConfigurationValues();

	// Implemented by derived classes for loading / writing.
	virtual bool LoadConfigValues(std::vector<std::string>& lines) = 0;
	virtual void WriteConfigValues() = 0;

public:
	int configVersion;

    ConfigManager(const char* configFileName);
    bool ReadConfiguration();
    bool WriteConfiguration();
};

