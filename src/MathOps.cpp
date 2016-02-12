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
    return (point[0] > minPoint[0] && point[1] > minPoint[1] && point[2] > minPoint[2] &&
            point[0] < maxPoint[0] && point[1] < maxPoint[1] && point[2] < maxPoint[2]);
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

