#pragma once
#include <random>
#include "Vec.h"

// Performs general physics and useful mathematical operations.
class MathOps
{
    private:
        // Random number generation
        static std::mt19937 mersenneTwiser;
        static uint32_t seedValue;
        static std::uniform_real_distribution<float> uniformDistribution;

    public:
        // Sets up static data
        MathOps();

        // Goes from radians to degrees
        static float Degrees(float angleInRadians);

        // Goes from degrees to radians
        static float Radians(float angleInDegrees);

        // Determines if the given point is within the cube specified by the min and max point.
        static bool WithinRange(vec::vec3 point, vec::vec3 minPoint, vec::vec3 maxPoint);
        static bool WithinRange(vec::vec2 point, vec::vec2 minPoint, vec::vec2 maxPoint);

        // Gets the next random floating-point value from 0 to 1, inclusive.
        static float Rand();

        // Gets the next random integer value from min(inclusive) to max(exclusive)
        static int Rand(int min, int max);

        // Gets the next random floating-point value from (-range/2 to range/2)
        static float Rand(float range);

        // Given two values, returns the smallest positive one.
        // If both are negative, returns 'first'.
        static float SmallestPositive(float first, float second);
};
