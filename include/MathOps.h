#pragma once
#include "vmath.hpp"

// Performs general physics and useful mathematical operations.
class MathOps
{
    public:
        // Goes from radians to degrees
        static float Degrees(float angleInRadians);

        // Goes from degrees to radians
        static float Radians(float angleInDegrees);

        // Determines if the given point is within the cube specified by the min and max point.
        static bool WithinRange(vmath::vec3 point, vmath::vec3 minPoint, vmath::vec3 maxPoint);
};
