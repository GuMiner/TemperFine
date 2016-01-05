#pragma once
#include <string>
#include <SFML\Window.hpp>
#include "vmath.hpp"

class ConversionUtils
{
    public:
        static bool LoadBool(const std::string& line, bool& boolean);
        static bool LoadInt(const std::string& line, int& integer);
        static bool LoadFloat(const std::string& line, float& floatingPoint);
        static bool LoadKey(const std::string& line, sf::Keyboard::Key& key);
        static bool LoadVector(const std::string& line, vmath::vec3& vector);
};
