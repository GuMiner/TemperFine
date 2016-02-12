#pragma once
#include "vmath.hpp"

// Performs vector-specific mathematical operations.
class VecOps
{
    public:
        // Cross-product.
        static vmath::vec3 Cross(const vmath::vec3& first, const vmath::vec3& second);

        // Dot-product.
        static float Dot(const vmath::vec3& first, const vmath::vec3& second);
};
