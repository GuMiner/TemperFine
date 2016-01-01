#pragma once
#include <string>

// Holds map information
struct MapInfo
{
    std::string name;
    unsigned int mapConfigVersion;

    unsigned int xSize;
    unsigned int ySize;
    unsigned int zSize;
    unsigned short* blockType;
    unsigned short* blockOrientation;
    unsigned short* blockProperty;
};
