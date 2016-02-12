#include "Constants.h"

float Constants::FOV_Y;
float Constants::ASPECT;
float Constants::NEAR_PLANE;
float Constants::FAR_PLANE;
vec::mat4 Constants::PerspectiveMatrix;

Constants::Constants()
{
    FOV_Y = 50.0f;
    ASPECT = 1.77778f; // 16:9
    NEAR_PLANE = 0.10f;
    FAR_PLANE = 1000.0f;

    PerspectiveMatrix = MatrixOps::Perspective(Constants::FOV_Y, Constants::ASPECT, Constants::NEAR_PLANE, Constants::FAR_PLANE);
}
