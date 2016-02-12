#include "ConversionUtils.h"
#include "StringUtils.h"

// Loads in a boolean configuration value.
bool ConversionUtils::LoadBool(const std::string& line, bool& boolean)
{
    std::string tempInput;
    return !(!StringUtils::SplitAndGrabSecondary(line, tempInput) || !StringUtils::ParseBoolFromString(tempInput, boolean));
}

// Loads in an integer configuration value.
bool ConversionUtils::LoadInt(const std::string& line, int& integer)
{
    std::string tempInput;
    return !(!StringUtils::SplitAndGrabSecondary(line, tempInput) || !StringUtils::ParseIntFromString(tempInput, integer));
}

// Loads in a floating-point configuration value.
bool ConversionUtils::LoadFloat(const std::string& line, float& floatingPoint)
{
    std::string tempInput;
    return !(!StringUtils::SplitAndGrabSecondary(line, tempInput) || !StringUtils::ParseFloatFromString(tempInput, floatingPoint));
}

// Loads in an SFML keyboard key.
bool ConversionUtils::LoadKey(const std::string& line, sf::Keyboard::Key& key)
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
bool ConversionUtils::LoadVector(const std::string& line, vec::vec3& vector)
{
	std::vector<std::string> stringParts;
	StringUtils::Split(line, StringUtils::Space, true, stringParts);

	if (stringParts.size() != 4)
	{
		return false;
	}

	if (!StringUtils::ParseFloatFromString(stringParts[1], vector.x) ||
		!StringUtils::ParseFloatFromString(stringParts[2], vector.y) ||
		!StringUtils::ParseFloatFromString(stringParts[3], vector.z))
	{
		return false;
	}

	return true;
}
