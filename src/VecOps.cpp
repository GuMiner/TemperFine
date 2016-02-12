#include "VecOps.h"

vmath::vec3 VecOps::Cross(const vmath::vec3& first, const vmath::vec3& second)
{
    return vmath::vec3(
        first[1] * second[2] - second[1] * first[2],
        first[2] * second[0] - second[2] * first[0],
        first[0] * second[1] - second[0] * first[1]);
}

float VecOps::Dot(const vmath::vec3& first, const vmath::vec3& second)
{
    vmath::vec3 multiplied = first * second;
    return multiplied[0] + multiplied[1] + multiplied[2];
}
