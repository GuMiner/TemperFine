#include "MathOps.h"
#include "MatrixOps.h"
#include "PhysicsOps.h"
#include "VecOps.h"

vec::vec3 PhysicsOps::XY_PLANE_NORMAL;
vec::vec3 PhysicsOps::YZ_PLANE_NORMAL;
vec::vec3 PhysicsOps::XZ_PLANE_NORMAL;

// Determines if a given ray hits a given plane. Returns true and fills in the intersection factor ('t', where rs + t*ray = intersection point).
bool PhysicsOps::HitsPlane(const vec::vec3& rayStart, const vec::vec3& ray, const vec::vec3& planeNormal, const vec::vec3& planePoint, float* intersectionFactor)
{
    float rayDotNormal = VecOps::Dot(ray, planeNormal);
    if (rayDotNormal == 0)
    {
        return false;
    }

    *intersectionFactor = VecOps::Dot(planePoint - rayStart, planeNormal) / rayDotNormal;
    return true;
}

bool PhysicsOps::HitsPlane(const vec::vec3& rayStart, const vec::vec3& ray, Plane plane, const vec::vec3& planePoint, float* intersectionFactor)
{
    vec::vec3 actualPlane;
    switch (plane)
    {
    case Plane::XY:
        actualPlane = XY_PLANE_NORMAL;
        break;
    case Plane::YZ:
        actualPlane = YZ_PLANE_NORMAL;
        break;
    case Plane::XZ:
    default:
        actualPlane = XZ_PLANE_NORMAL;
        break;
    }

    return HitsPlane(rayStart, ray, actualPlane, planePoint, intersectionFactor);
}

bool PhysicsOps::WithinSquare(const vec::vec3& position, Plane plane, const vec::vec3& minPosition, const vec::vec3& maxPosition)
{
    switch (plane)
    {
    case Plane::XY:
        return MathOps::WithinRange(vec::vec2(position.x, position.y),
            vec::vec2(minPosition.x, minPosition.y), vec::vec2(maxPosition.x, maxPosition.y));
    case Plane::YZ:
        return MathOps::WithinRange(vec::vec2(position.y, position.z),
            vec::vec2(minPosition.y, minPosition.z), vec::vec2(maxPosition.y, maxPosition.z));
    case Plane::XZ:
    default:
        return MathOps::WithinRange(vec::vec2(position.x, position.z),
            vec::vec2(minPosition.x, minPosition.z), vec::vec2(maxPosition.x, maxPosition.z));
    }
}

bool PhysicsOps::HitsSphere(const vec::vec3& rayStart, const vec::vec3& ray, const vec::vec3& sphereCenter, float sphereRadius)
{
    // After some work on paper, it ends up that you just need to calculate a determinant and ensure it is > 0
    vec::vec3 rayOffset = rayStart - sphereCenter;

    float first = pow(VecOps::Dot(ray, rayOffset), 2);
    float second = VecOps::Dot(rayOffset, rayOffset) - pow(sphereRadius, 2);

    return first >= second; // first - second >= 0
}

// Computes a ray from the current mouse position into the scene.
vec::vec3 PhysicsOps::ScreenRay(vec::vec2 mouse, vec::vec2 screenSize, vec::mat4& perspectiveMatrix, vec::mat4& viewRotationMatrix)
{
    // Scale from -1.0 to 1.0, and invert Y
    vec::vec2 deviceCoords = (mouse * 2.0f - screenSize) / screenSize;
    deviceCoords.y = -deviceCoords.y;

    // Point the ray away from us.
    vec::vec4 clippedRay = vec::vec4(deviceCoords.x, deviceCoords.y, -1.0f, 1.0f);

    // Invert our projection and use the normal view matrix to end up with a world ray, which is exactly what we want.
    vec::mat4 invPerspectiveMatrix;
    MatrixOps::Inverse(perspectiveMatrix, invPerspectiveMatrix);
    vec::vec4 eyeRay = clippedRay * invPerspectiveMatrix;
    eyeRay[2] = -1.0f;
    eyeRay[3] = 0.0f;

    vec::vec4 worldRay = eyeRay * viewRotationMatrix;

    vec::vec3 result = vec::vec3(worldRay[0], worldRay[1], worldRay[2]);
    return normalize(result);
}
