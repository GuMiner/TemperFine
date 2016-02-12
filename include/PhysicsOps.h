#pragma once
#include "Vec.h"

// Holds physics-specific mathematical operations
class PhysicsOps
{
    public:
        // Determines if a given ray hits a given plane. Returns true and fills in the intersection factor ('t', where rs + t*ray = intersection point).
        static bool HitsPlane(const vec::vec3& rayStart, const vec::vec3& ray, const vec::vec3& planeNormal, const vec::vec3& planePoint, float* intersectionFactor);

        // Computes a ray from the current mouse position into the scene.
        static vec::vec3 ScreenRay(vec::vec2 mouse, vec::vec2 screenSize, vec::mat4& perspectiveMatrix, vec::mat4& viewRotationMatrix);
};

