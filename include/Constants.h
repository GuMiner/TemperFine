#pragma once
#include <random>

// Holds data constant across the application.
class Constants
{
    // Random number generation
    static std::mt19937 mersenneTwiser;
    static uint32_t seedValue;
    static std::uniform_real_distribution<float> uniformDistribution;

public:
    Constants()
    {
        FOV_Y = 50.0f;
        ASPECT = 1.77778f; // 16:9
        NEAR_PLANE = 0.10f;
        FAR_PLANE = 1000.0f;
        seedValue = 3213258; // Numpad-bashing.
        mersenneTwiser.seed(seedValue);
    }

    // Allowable return codes
    enum Status { OK = 0, BAD_SHADERS = 1, BAD_FONT = 2,
        BAD_IMAGES = 3, BAD_SOUND = 4, BAD_MUSIC = 5,
        BAD_CONFIG = 6, BAD_GLEW = 7 };

    // Graphics viewport settings
    static float FOV_Y;
    static float ASPECT;
    static float NEAR_PLANE;
    static float FAR_PLANE;

    // To support 144 Hz monitors.
    const static int MAX_FRAMERATE = 150;

    // Gets the next random floating-point value from 0 to 1, inclusive.
    static float Rand();

    // Gets the next random integer value from min(inclusive) to max(exclusive)
    static int Rand(int min, int max);

    // Gets the next random floating-point value from (-range/2 to range/2)
    static float Rand(float range);
};

