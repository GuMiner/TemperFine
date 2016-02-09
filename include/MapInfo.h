#pragma once
#include <string>

// Holds map information
struct MapInfo
{
    enum BlockTypes
    {
        AIR = 0,
        CUBE = 1,
        SLANT = 2
    };

    std::string name;
    unsigned int mapConfigVersion;

    unsigned int xSize;
    unsigned int ySize;
    unsigned int zSize;
    unsigned char* blockType;
    unsigned char* blockOrientation;
    unsigned char* blockProperty;

    // Gets the voxels in the map, including empty ones!
    inline int GetVoxelCount() const
    {
        return xSize * ySize * zSize;
    }

    // Gets the index of a voxel on the map as an array offset.
    inline int GetIndex(unsigned int x, unsigned int y, unsigned int z) const
    {
        return z*xSize*ySize + y*xSize + x;
    }

    // Gets index when using a provided x and y size.
    static int GetIndex(unsigned int x, unsigned int y, unsigned int z, unsigned int xSize, unsigned int ySize)
    {
        return z*xSize*ySize + y*xSize + x;
    }
};
