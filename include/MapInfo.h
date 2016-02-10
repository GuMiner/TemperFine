#pragma once
#include <string>
#include "vmath.hpp"

// Holds map information
struct MapInfo
{
    enum VoxelTypes
    {
        AIR = 0,
        CUBE = 1,
        SLANT = 2
    };

    // Size of each voxel. MUST match that in the voxel shader.
    const float SPACING = 2.0f;

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

    inline int GetIndex(const vmath::vec3i& voxelId) const
    {
        return GetIndex(voxelId.x, voxelId.y, voxelId.z);
    }

    inline bool InBounds(const vmath::vec3i& voxelId) const
    {
        return (voxelId.x >= 0 && voxelId.y >= 0 && voxelId.z >= 0 && voxelId.x < (int)xSize && voxelId.y < (int)ySize && voxelId.z < (int)zSize);
    }

    inline int GetType(const vmath::vec3i& voxelId) const
    {
        return blockType[GetIndex(voxelId)];
    }

    inline int GetOrientation(const vmath::vec3i& voxelId) const
    {
        return blockOrientation[GetIndex(voxelId)];
    }

    // Gets index when using a provided x and y size.
    static int GetIndex(unsigned int x, unsigned int y, unsigned int z, unsigned int xSize, unsigned int ySize)
    {
        return z*xSize*ySize + y*xSize + x;
    }

    MapInfo()
    {
    }
};
