#include "MathOps.h"
#include "MatrixOps.h"

std::mt19937 MathOps::mersenneTwiser;
uint32_t MathOps::seedValue;
std::uniform_real_distribution<float> MathOps::uniformDistribution;

MathOps::MathOps()
{
    seedValue = 3213258; // Numpad-bashing.
    mersenneTwiser.seed(seedValue);
}

float MathOps::Degrees(float angleInRadians)
{
    return angleInRadians * (180.0f / 3.14159f);
}

// Goes from degrees to radians
float MathOps::Radians(float angleInDegrees)
{
    return angleInDegrees * (3.14159f / 180.0f);
}

// Determines if the given point is within the cube specified by the min and max point.
bool MathOps::WithinRange(vec::vec3 point, vec::vec3 minPoint, vec::vec3 maxPoint)
{
    return (point.x > minPoint.x && point.y > minPoint.y && point.z > minPoint.z &&
            point.x < maxPoint.x && point.y < maxPoint.y && point.z < maxPoint.z);
}

float MathOps::Rand()
{
    return uniformDistribution(mersenneTwiser);
}

int MathOps::Rand(int min, int max)
{
    return std::uniform_int_distribution<int>{min, max - 1}(mersenneTwiser);
}

float MathOps::Rand(float range)
{
    return range*Rand() - range*0.5f;
}

