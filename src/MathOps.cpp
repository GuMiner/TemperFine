#include "MathOps.h"
#include "MatrixOps.h"

float MathOps::Degrees(float angleInRadians)
{
    // TODO embed constant in Constants and move random number calculation here.
    return angleInRadians * (180.0f / 3.14159f);
}

// Goes from degrees to radians
float MathOps::Radians(float angleInDegrees)
{
    return angleInDegrees * (3.14159f / 180.0f);
}

// Determines if the given point is within the cube specified by the min and max point.
bool MathOps::WithinRange(vmath::vec3 point, vmath::vec3 minPoint, vmath::vec3 maxPoint)
{
    return (point[0] > minPoint[0] && point[1] > minPoint[1] && point[2] > minPoint[2] &&
            point[0] < maxPoint[0] && point[1] < maxPoint[1] && point[2] < maxPoint[2]);
}
