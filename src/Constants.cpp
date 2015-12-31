#include "Constants.h"

std::mt19937 Constants::mersenneTwiser;
uint32_t Constants::seedValue;
std::uniform_real_distribution<float> Constants::uniformDistribution;

float Constants::FOV_Y;
float Constants::ASPECT;
float Constants::NEAR_PLANE;
float Constants::FAR_PLANE;

float Constants::Rand()
{
    return uniformDistribution(mersenneTwiser);
}

int Constants::Rand(int min, int max)
{
    return std::uniform_int_distribution<int>{min, max - 1}(mersenneTwiser);
}

float Constants::Rand(float range)
{
    return range*Rand() - range*0.5f;
}