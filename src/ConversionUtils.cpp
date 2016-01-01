#include "ConversionUtils.h"
#include "StringUtils.h"

// Loads in a boolean configuration value.
bool ConversionUtils::LoadBool(std::string& line, bool& boolean)
{
    std::string tempInput;
    return !(!StringUtils::SplitAndGrabSecondary(line, tempInput) || !StringUtils::ParseBoolFromString(tempInput, boolean));
}

// Loads in an integer configuration value.
bool ConversionUtils::LoadInt(std::string& line, int& integer)
{
    std::string tempInput;
    return !(!StringUtils::SplitAndGrabSecondary(line, tempInput) || !StringUtils::ParseIntFromString(tempInput, integer));
}

// Loads in a floating-point configuration value.
bool ConversionUtils::LoadFloat(std::string& line, float& floatingPoint)
{
    std::string tempInput;
    return !(!StringUtils::SplitAndGrabSecondary(line, tempInput) || !StringUtils::ParseFloatFromString(tempInput, floatingPoint));
}

// Loads in an SFML keyboard key.
bool ConversionUtils::LoadKey(std::string& line, sf::Keyboard::Key& key)
{
    int keyInt;
    if (!LoadInt(line, keyInt))
    {
        return false;
    }

    key = (sf::Keyboard::Key)keyInt;
    return true;
}

// Loads in a 3-valued floating point vector.
bool ConversionUtils::LoadVector(std::string& line, vmath::vec3& vector)
{
	std::vector<std::string> stringParts;
	StringUtils::Split(line, StringUtils::Space, true, stringParts);

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
