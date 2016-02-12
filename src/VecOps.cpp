#include "VecOps.h"

vec::vec3 VecOps::Cross(const vec::vec3& first, const vec::vec3& second)
{
    return vec::vec3(
        first[1] * second[2] - second[1] * first[2],
        first[2] * second[0] - second[2] * first[0],
        first[0] * second[1] - second[0] * first[1]);
}

float VecOps::Dot(const vec::vec3& first, const vec::vec3& second)
{
    vec::vec3 multiplied = first * second;
    return multiplied[0] + multiplied[1] + multiplied[2];
}
