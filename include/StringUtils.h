#pragma once

#include <string>
#include <vector>

class StringUtils
{
public:
    static const char Newline = '\n';
    static const char Space = ' ';

    // Writes a really big string to a file; returns true on success.
    static bool WriteStringToFile(const std::string& filename, std::string& data);

    // Loads a file as a really big string; returns true on success.
    static bool LoadStringFromFile(const std::string& filename, std::string& result);

    // Splits a string into substrings, optionally including empty tokens if present.
    static void Split(const std::string& stringToSplit, char delimator, bool excludeEmpty, std::vector<std::string>& splitStrings);

    // Splits a line into two parts and grabs the secondary part.
    static bool SplitAndGrabSecondary(const std::string& line, std::string& secondary);

    // Determines if the given string starts with the provided sub string.
    static bool StartsWith(const std::string& givenString, const std::string& subString);

    // Determines if the given string is completely empty or whitespace.
    static bool IsWhitespaceOrEmpty(const std::string& givenString);

    // Attempts to parse a boolean from a string.
    static bool ParseBoolFromString(const std::string& stringToParse, bool& value);

    // Attempts to parse an integer from a string.
    static bool ParseIntFromString(const std::string& stringToParse, int& value);

    // Attempts to parse a double from a string.
    static bool ParseFloatFromString(const std::string& stringToParse, float& value);
};