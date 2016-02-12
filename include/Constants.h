#pragma once
#include "MatrixOps.h"

// Holds data constant across the application.
class Constants
{
public:
    Constants();

    // Allowable return codes
    enum Status { OK = 0, BAD_SHADERS = 1, BAD_FONT = 2,
        BAD_IMAGES = 3, BAD_SOUND = 4, BAD_MUSIC = 5,
        BAD_CONFIG = 6, BAD_GLEW = 7, BAD_STATS = 8, BAD_VOXEL_MAP = 9,
        BAD_MAP = 10, BAD_UI = 11, BAD_SCENERY = 12, BAD_ROUTER = 13 };

    // Graphics viewport settings
    static float FOV_Y;
    static float ASPECT;
    static float NEAR_PLANE;
    static float FAR_PLANE;
    static vec::mat4 PerspectiveMatrix;

    // To support 144 Hz monitors.
    const static int MAX_FRAMERATE = 150;
};

