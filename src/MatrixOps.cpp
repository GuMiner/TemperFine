#include "MathOps.h"
#include "VecOps.h"
#include "MatrixOps.h"

vec::mat4 MatrixOps::Perspective(float fovy, float aspect, float n, float f)
{
    float q = 1.0f / tanf(MathOps::Radians(0.5f * fovy));
    float A = q / aspect;
    float B = (n + f) / (n - f);
    float C = (2.0f * n * f) / (n - f);

    vec::mat4 result;
    result[0] = vec::vec4(A, 0.0f, 0.0f, 0.0f);
    result[1] = vec::vec4(0.0f, q, 0.0f, 0.0f);
    result[2] = vec::vec4(0.0f, 0.0f, B, -1.0f);
    result[3] = vec::vec4(0.0f, 0.0f, C, 0.0f);
    return result;
}

vec::mat4 MatrixOps::Lookat(const vec::vec3& eye, const vec::vec3& center, const vec::vec3& up)
{
    const vec::vec3 f = normalize(center - eye);
    const vec::vec3 upN = normalize(up);
    const vec::vec3 s = VecOps::Cross(f, upN);
    const vec::vec3 u = VecOps::Cross(s, f);
    const vec::mat4 M = vec::mat4(
        vec::vec4(s[0], u[0], -f[0], 0.0f),
        vec::vec4(s[1], u[1], -f[1], 0.0f),
        vec::vec4(s[2], u[2], -f[2], 0.0f),
        vec::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    return M * Translate(-eye);
}

vec::mat4 MatrixOps::Translate(float x, float y, float z)
{
    return vec::mat4(
        vec::vec4(1.0f, 0.0f, 0.0f, 0.0f),
        vec::vec4(0.0f, 1.0f, 0.0f, 0.0f),
        vec::vec4(0.0f, 0.0f, 1.0f, 0.0f),
        vec::vec4(x, y, z, 1.0f));
}

vec::mat4 MatrixOps::Translate(const vec::vec3& v)
{
    return Translate(v[0], v[1], v[2]);
}

vec::mat4 MatrixOps::Scale(float x, float y, float z)
{
    return vec::mat4(
        vec::vec4(x, 0.0f, 0.0f, 0.0f),
        vec::vec4(0.0f, y, 0.0f, 0.0f),
        vec::vec4(0.0f, 0.0f, z, 0.0f),
        vec::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

vec::mat4 MatrixOps::Scale(const vec::vec3& v)
{
    return Scale(v[0], v[1], v[2]);
}

vec::mat4 MatrixOps::Rotate(float angle, float x, float y, float z)
{
    vec::mat4 result;

    const float x2 = x * x;
    const float y2 = y * y;
    const float z2 = z * z;
    float rads = float(angle) * 0.0174532925f;
    const float c = cosf(rads);
    const float s = sinf(rads);
    const float omc = 1.0f - c;

    result[0] = vec::vec4(x2 * omc + c, y * x * omc + z * s, x * z * omc - y * s, 0.0f);
    result[1] = vec::vec4(x * y * omc - z * s, y2 * omc + c, y * z * omc + x * s, 0.0f);
    result[2] = vec::vec4(x * z * omc + y * s, y * z * omc - x * s, z2 * omc + c, 0.0f);
    result[3] = vec::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    return result;
}

vec::mat4 MatrixOps::Rotate(float angle, const vec::vec3& v)
{
    return Rotate(angle, v[0], v[1], v[2]);
}

// Pulled from 'The Mesa 3-D graphics library', gluInvertMatrix, refactored to integrate here.
// Computes the inverse of the provided 4x4 matrix. *Assumes the matrix is invertable*
void MatrixOps::Inverse(const vec::mat4& matrix, vec::mat4& result)
{
    // Reading directly from the matrix.
    const float* m = &matrix[0][0];
    float inv[16];

    inv[0] = m[5]  * m[10] * m[15] -
             m[5]  * m[11] * m[14] -
             m[9]  * m[6]  * m[15] +
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] -
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] +
              m[4]  * m[11] * m[14] +
              m[8]  * m[6]  * m[15] -
              m[8]  * m[7]  * m[14] -
              m[12] * m[6]  * m[11] +
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] -
             m[4]  * m[11] * m[13] -
             m[8]  * m[5] * m[15] +
             m[8]  * m[7] * m[13] +
             m[12] * m[5] * m[11] -
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] +
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] -
               m[8]  * m[6] * m[13] -
               m[12] * m[5] * m[10] +
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] +
              m[1]  * m[11] * m[14] +
              m[9]  * m[2] * m[15] -
              m[9]  * m[3] * m[14] -
              m[13] * m[2] * m[11] +
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] -
             m[0]  * m[11] * m[14] -
             m[8]  * m[2] * m[15] +
             m[8]  * m[3] * m[14] +
             m[12] * m[2] * m[11] -
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] +
              m[0]  * m[11] * m[13] +
              m[8]  * m[1] * m[15] -
              m[8]  * m[3] * m[13] -
              m[12] * m[1] * m[11] +
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] -
              m[0]  * m[10] * m[13] -
              m[8]  * m[1] * m[14] +
              m[8]  * m[2] * m[13] +
              m[12] * m[1] * m[10] -
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] -
             m[1]  * m[7] * m[14] -
             m[5]  * m[2] * m[15] +
             m[5]  * m[3] * m[14] +
             m[13] * m[2] * m[7] -
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] +
              m[0]  * m[7] * m[14] +
              m[4]  * m[2] * m[15] -
              m[4]  * m[3] * m[14] -
              m[12] * m[2] * m[7] +
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] -
              m[0]  * m[7] * m[13] -
              m[4]  * m[1] * m[15] +
              m[4]  * m[3] * m[13] +
              m[12] * m[1] * m[7] -
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] +
               m[0]  * m[6] * m[13] +
               m[4]  * m[1] * m[14] -
               m[4]  * m[2] * m[13] -
               m[12] * m[1] * m[6] +
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
              m[1] * m[7] * m[10] +
              m[5] * m[2] * m[11] -
              m[5] * m[3] * m[10] -
              m[9] * m[2] * m[7] +
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
             m[0] * m[7] * m[10] -
             m[4] * m[2] * m[11] +
             m[4] * m[3] * m[10] +
             m[8] * m[2] * m[7] -
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
               m[0] * m[7] * m[9] +
               m[4] * m[1] * m[11] -
               m[4] * m[3] * m[9] -
               m[8] * m[1] * m[7] +
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
              m[0] * m[6] * m[9] -
              m[4] * m[1] * m[10] +
              m[4] * m[2] * m[9] +
              m[8] * m[1] * m[6] -
              m[8] * m[2] * m[5];

    float determinant = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (determinant == 0)
    {
        // Yes, this introduces incorrect results. However, if we get into a situation
        //  where the matrix isn't invertable, we've got a bigger problem.
        determinant = 0.001f;
    }

    determinant = 1.0 / determinant;

    // Convert our data directly back to a matrix.
    for (unsigned int i = 0; i < 16; i++)
    {
        *(&result[0][0] + i) = inv[i] * determinant;
    }
}
