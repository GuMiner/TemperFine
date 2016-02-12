#include <cmath>
#include "VecOps.h"

vec::vec3 VecOps::Cross(const vec::vec3& first, const vec::vec3& second)
{
    return vec::vec3(
        first.y * second.z - second.y * first.z,
        first.z * second.x - second.z * first.x,
        first.x * second.y - second.x * first.y);
}

float VecOps::Dot(const vec::vec3& first, const vec::vec3& second)
{
    vec::vec3 multiplied = first * second;
    return multiplied.x + multiplied.y + multiplied.z;
}

// Distance between two vectors
float VecOps::Distance(const vec::vec3& first, const vec::vec3& second)
{
    return vec::length(second - first);
}

// Angle between two vectors.
float VecOps::Angle(const vec::vec3& first, const vec::vec3& second)
{
    return std::acos(Dot(first, second));
}
