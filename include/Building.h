#pragma once
#include "Vec.h"

// Represents an in-game building that is placed on the nine possible locations of a voxel.
class Building
{
public:
    // What position of the tic-tac-toe nine possible lcoations the building is in.
    enum Sector
    {
        XPYP, // X+, Y+
        XMYM, // X-, Y-
        XPYM,
        XMYP,
        XP,
        YP,
        XM,
        YM,
        CENTER
    };

private:
    // What voxel the building is on top of, and where the building is.
    vec::vec3i voxelId;
    Sector sector;
};