#pragma once
#include "Vec.h"

// Holds physics-specific mathematical operations
class PhysicsOps
{
    public:
        enum Plane
        {
            XY = 0,
            YZ,
            XZ
        };

        static vec::vec3 XY_PLANE_NORMAL;
        static vec::vec3 YZ_PLANE_NORMAL;
        static vec::vec3 XZ_PLANE_NORMAL;

        PhysicsOps()
        {
            XY_PLANE_NORMAL = vec::vec3(0.0f, 0.0f, 1.0f);
            YZ_PLANE_NORMAL = vec::vec3(1.0f, 0.0f, 0.0f);
            XZ_PLANE_NORMAL = vec::vec3(0.0f, 1.0f, 0.0f);
        }

        // Determines if a given ray hits a given plane. Returns true and fills in the intersection factor ('t', where rs + t*ray = intersection point).
        static bool HitsPlane(const vec::vec3& rayStart, const vec::vec3& ray, const vec::vec3& planeNormal, const vec::vec3& planePoint, float* intersectionFactor);
        static bool HitsPlane(const vec::vec3& rayStart, const vec::vec3& ray, Plane plane, const vec::vec3& planePoint, float* intersectionFactor);

        // Determines if the given point is within a square on the plane.
        // Note that min/max position expect their x/y/z arguments in the same order as written in Plane, ie XY => (x, y) and XZ => (x, z)
        static bool WithinSquare(const vec::vec3& position, Plane plane, const vec::vec3& minPosition, const vec::vec3& maxPosition);

        // Determines if a given ray hits a sphere. Returns true if so, false otherwise.
        static bool HitsSphere(const vec::vec3& rayStart, const vec::vec3& ray, const vec::vec3& sphereCenter, float sphereRadius);

        // Computes a ray from the current mouse position into the scene.
        static vec::vec3 ScreenRay(vec::vec2 mouse, vec::vec2 screenSize, vec::mat4& perspectiveMatrix, vec::mat4& viewRotationMatrix);
};

