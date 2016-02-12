#include "MatrixOps.h"
#include "PhysicsOps.h"

// Determines if a given ray hits a given plane. Returns true and fills in the intersection factor ('t', where rs + t*ray = intersection point).
bool PhysicsOps::HitsPlane(const vmath::vec3& rayStart, const vmath::vec3& ray, const vmath::vec3& planeNormal, const vmath::vec3& planePoint, float* intersectionFactor)
{
    // TODO
    return false;
}

// Computes a ray from the current mouse position into the scene.
vmath::vec3 PhysicsOps::ScreenRay(vmath::vec2 mouse, vmath::vec2 screenSize, vmath::mat4& perspectiveMatrix, vmath::mat4& viewRotationMatrix)
{
    // Scale from -1.0 to 1.0, and invert Y
    vmath::vec2 deviceCoords = (mouse * 2.0f - screenSize) / screenSize;
    deviceCoords[1] = -deviceCoords[1];

    // Point the ray away from us.
    vmath::vec4 clippedRay = vmath::vec4(deviceCoords[0], deviceCoords[1], -1.0f, 1.0f);

    // Invert our projection and use the normal view matrix to end up with a world ray, which is exactly what we want.
    vmath::mat4 invPerspectiveMatrix;
    MatrixOps::Inverse(perspectiveMatrix, invPerspectiveMatrix);
    vmath::vec4 eyeRay = clippedRay * invPerspectiveMatrix;
    eyeRay[2] = -1.0f;
    eyeRay[3] = 0.0f;

    vmath::vec4 worldRay = eyeRay * viewRotationMatrix;

    vmath::vec3 result = vmath::vec3(worldRay[0], worldRay[1], worldRay[2]);
    return normalize(result);
}
