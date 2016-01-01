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

    // Simplifies index calculation.
    inline int GetIndex(unsigned int x, unsigned int y, unsigned int z)
    {
        return z*xSize*ySize + y*xSize + x;
    }

    // Gets index when using a provided x and y size.
    static int GetIndex(unsigned int x, unsigned int y, unsigned int z, unsigned int xSize, unsigned int ySize)
    {
        return z*xSize*ySize + y*xSize + x;
    }
};
