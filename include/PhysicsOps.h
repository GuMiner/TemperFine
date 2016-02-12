#pragma once
#include "vmath.hpp"

// Holds physics-specific mathematical operations
class PhysicsOps
{
    public:
        // Determines if a given ray hits a given plane. Returns true and fills in the intersection factor ('t', where rs + t*ray = intersection point).
        static bool HitsPlane(const vmath::vec3& rayStart, const vmath::vec3& ray, const vmath::vec3& planeNormal, const vmath::vec3& planePoint, float* intersectionFactor);

        // Computes a ray from the current mouse position into the scene.
        static vmath::vec3 ScreenRay(vmath::vec2 mouse, vmath::vec2 screenSize, vmath::mat4& perspectiveMatrix, vmath::mat4& viewRotationMatrix);
};

