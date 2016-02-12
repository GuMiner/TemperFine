#pragma once
#include "vmath.hpp"

// Performs matrix-specific math operations.
class MatrixOps
{
    public:
        // Computes a perspective matrix with the provided FOV-Y, aspect, near plane, and far plane.
        static vmath::mat4 Perspective(float fovy, float aspect, float n, float f);

        // Computes a look-at matrix
        static vmath::mat4 Lookat(const vmath::vec3& eye, const vmath::vec3& center, const vmath::vec3& up);

        // Computes a translation matrix.
        static vmath::mat4 Translate(float x, float y, float z);
        static vmath::mat4 Translate(const vmath::vec3& v);

        // Computes a scaling matrix.
        static vmath::mat4 Scale(float x, float y, float z);
        static vmath::mat4 Scale(const vmath::vec3& v);

        // Computes an axis-angle rotation matrix.
        static vmath::mat4 Rotate(float angle, float x, float y, float z);
        static vmath::mat4 Rotate(float angle, const vmath::vec3& v);

        // Computes the inverse of the provided matrix, filling it into the result.
        static void Inverse(const vmath::mat4& matrix, vmath::mat4& result);
};
