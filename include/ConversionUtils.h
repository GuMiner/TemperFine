#pragma once
#include <string>
#include <SFML\Window.hpp>
#include "vmath.hpp"

class ConversionUtils
{
    public:
        static bool LoadBool(std::string& line, bool& boolean);
        static bool LoadInt(std::string& line, int& integer);
        static bool LoadFloat(std::string& line, float& floatingPoint);
        static bool LoadKey(std::string& line, sf::Keyboard::Key& key);
        static bool LoadVector(std::string& line, vmath::vec3& vector);
};
