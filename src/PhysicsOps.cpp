#include "MatrixOps.h"
#include "PhysicsOps.h"

// Determines if a given ray hits a given plane. Returns true and fills in the intersection factor ('t', where rs + t*ray = intersection point).
bool PhysicsOps::HitsPlane(const vec::vec3& rayStart, const vec::vec3& ray, const vec::vec3& planeNormal, const vec::vec3& planePoint, float* intersectionFactor)
{
    // TODO
    return false;
}

// Computes a ray from the current mouse position into the scene.
vec::vec3 PhysicsOps::ScreenRay(vec::vec2 mouse, vec::vec2 screenSize, vec::mat4& perspectiveMatrix, vec::mat4& viewRotationMatrix)
{
    // Scale from -1.0 to 1.0, and invert Y
    vec::vec2 deviceCoords = (mouse * 2.0f - screenSize) / screenSize;
    deviceCoords[1] = -deviceCoords[1];

    // Point the ray away from us.
    vec::vec4 clippedRay = vec::vec4(deviceCoords[0], deviceCoords[1], -1.0f, 1.0f);

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
