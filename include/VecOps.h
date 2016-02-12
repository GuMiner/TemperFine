#pragma once
#include "Vec.h"

// Performs vector-specific mathematical operations.
class VecOps
{
    public:
        // Cross-product.
        static vec::vec3 Cross(const vec::vec3& first, const vec::vec3& second);

        // Dot-product.
        static float Dot(const vec::vec3& first, const vec::vec3& second);
};
