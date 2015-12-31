#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "StringUtils.h"

// Loads a file as a really big string; returns true on success.
bool StringUtils::LoadStringFromFile(const std::string& filename, std::string& result)
{
    std::ifstream file(filename.c_str());
    if (!file)
    {
        return false;
    }

    std::stringstream stream;
    stream << file.rdbuf();
    result = stream.str();

    file.close();
    return true;
}

// Writes a really big string to a file; returns true on success.
bool StringUtils::WriteStringToFile(const std::string& filename, std::string& data)
{
    std::ofstream file(filename.c_str());
    if (!file)
    {
        return false;
    }

    file << data.c_str();
    file.close();

    return true;
}

// Splits a string into substrings, optionally including empty tokens if present.
void StringUtils::Split(const std::string& stringToSplit, char delimator, bool excludeEmpty, std::vector<std::string>& splitStrings)
{
    splitStrings.clear();
    std::stringstream stringStream(stringToSplit);

    std::string item;
    while (std::getline(stringStream, item, delimator))
    {
        if (!excludeEmpty || (excludeEmpty && !item.empty()))
        {
            splitStrings.push_back(item);
        }
    }
}

// Splits a line into two parts and grabs the secondary part.
bool StringUtils::SplitAndGrabSecondary(const std::string& line, std::string& secondary)
{
    std::vector<std::string> stringParts;
    Split(line, Space, true, stringParts);

    if (stringParts.size() != 2)
    {
        return false;
    }

    secondary = stringParts[1];

    return true;
}

// Determines if the given string starts with the provided sub string.
bool StringUtils::StartsWith(const std::string& givenString, const std::string& subString)
{
    if (givenString.length() < subString.length())
    {
        return false;
    }

    return (strncmp(givenString.substr(0, subString.length()).c_str(), subString.c_str(), subString.length()) == 0);
}

// Determines if the given string is completely empty or whitespace.
bool StringUtils::IsWhitespaceOrEmpty(const std::string& givenString)
{
    if (givenString.length() == 0)
    {
        return true;
    }

    for (char character : givenString)
    {
        if (character != ' ' && character != '\r' && character != '\n' && character != '\t')
        {
            return false;
        }
    }
    return true;
}

// Attempts to parse a boolean from a string.
bool StringUtils::ParseBoolFromString(const std::string& stringToParse, bool& value)
{
    std::istringstream inputStream(stringToParse);
    return inputStream >> std::boolalpha >> value ? true : false;
}

// Attempts to parse an integer from a string.
bool StringUtils::ParseIntFromString(const std::string& stringToParse, int& value)
{
    std::istringstream inputStream(stringToParse);
    return inputStream >> value ? true : false;
}

// Attempts to parse a double from a string.
bool StringUtils::ParseFloatFromString(const std::string& stringToParse, float& value)
{
    double tempValue;
    std::istringstream inputStream(stringToParse);
    if (inputStream >> tempValue)
    {
        value = (float)tempValue;
        return true;
    }

    return false;
}
