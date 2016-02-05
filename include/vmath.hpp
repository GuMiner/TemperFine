#pragma once
#define _USE_MATH_DEFINES  1 // Include constants defined in math.h
#include <math.h>
#include <iostream>

// From the OpenGL Superbible, 6th Ed, 'free to use in your application' as per the website.
// Modified so that quaternions can actually be used and removing dead code.
namespace vmath
{
    // Goes from radians to degrees
    inline float degrees(float angleInRadians)
    {
        return angleInRadians * (float)(180.0 / 3.14159);
    }

    // Goes from degrees to radians
    inline float radians(float angleInDegrees)
    {
        return angleInDegrees * (float)(3.14159 / 180.0);
    }

    template <const int len>
    class vecN
    {
    public:
        typedef class vecN<len> my_type;
        typedef float element_type;

        // Default constructor does nothing, just like built-in types
        inline vecN()
        {
            // Uninitialized variable
        }

        // Copy constructor
        inline vecN(const vecN& that)
        {
            assign(that);
        }

        // Construction from scalar
        inline vecN(float s)
        {
            int n;
            for (n = 0; n < len; n++)
            {
                data[n] = s;
            }
        }

        // Assignment operator
        inline vecN& operator=(const vecN& that)
        {
            assign(that);
            return *this;
        }

        inline vecN& operator=(const float& that)
        {
            int n;
            for (n = 0; n < len; n++)
                data[n] = that;

            return *this;
        }

        inline vecN operator+(const vecN& that) const
        {
            my_type result;
            int n;
            for (n = 0; n < len; n++)
                result.data[n] = data[n] + that.data[n];
            return result;
        }

        inline vecN& operator+=(const vecN& that)
        {
            return (*this = *this + that);
        }

        inline vecN operator-() const
        {
            my_type result;
            int n;
            for (n = 0; n < len; n++)
                result.data[n] = -data[n];
            return result;
        }

        inline vecN operator-(const vecN& that) const
        {
            my_type result;
            int n;
            for (n = 0; n < len; n++)
                result.data[n] = data[n] - that.data[n];
            return result;
        }

        inline vecN& operator-=(const vecN& that)
        {
            return (*this = *this - that);
        }

        inline vecN operator*(const vecN& that) const
        {
            my_type result;
            int n;
            for (n = 0; n < len; n++)
                result.data[n] = data[n] * that.data[n];
            return result;
        }

        inline vecN& operator*=(const vecN& that)
        {
            return (*this = *this * that);
        }

        inline vecN operator*(const float& that) const
        {
            my_type result;
            int n;
            for (n = 0; n < len; n++)
                result.data[n] = data[n] * that;
            return result;
        }

        inline vecN& operator*=(const float& that)
        {
            assign(*this * that);

            return *this;
        }

        inline vecN operator/(const vecN& that) const
        {
            my_type result;
            int n;
            for (n = 0; n < len; n++)
                result.data[n] = data[n] / that.data[n];
            return result;
        }

        inline vecN& operator/=(const vecN& that)
        {
            assign(*this / that);

            return *this;
        }

        inline vecN operator/(const float& that) const
        {
            my_type result;
            int n;
            for (n = 0; n < len; n++)
                result.data[n] = data[n] / that;
            return result;
        }

        inline vecN& operator/=(const float& that)
        {
            assign(*this / that);
            return *this;
        }

        inline float& operator[](int n) { return data[n]; }
        inline const float& operator[](int n) const { return data[n]; }

        inline static int size(void) { return len; }

        inline operator const float* () const { return &data[0]; }

    protected:
        float data[len];

        inline void assign(const vecN& that)
        {
            int n;
            for (n = 0; n < len; n++)
                data[n] = that.data[n];
        }
    };

    class vec2 : public vecN<2>
    {
    public:
        typedef vecN<2> base;

        // Uninitialized variable
        inline vec2() {}

        // Copy constructor
        inline vec2(const base& v) : base(v) {}

        // vec2(x, y);
        inline vec2(float x, float y)
        {
            base::data[0] = x;
            base::data[1] = y;
        }
    };

    class vec3 : public vecN<3>
    {
    public:
        typedef vecN<3> base;

        // Uninitialized variable
        inline vec3() {}

        // Copy constructor
        inline vec3(const base& v) : base(v) {}

        // vec3(x, y, z);
        inline vec3(float x, float y, float z)
        {
            base::data[0] = x;
            base::data[1] = y;
            base::data[2] = z;
        }
    };

    class vec4 : public vecN<4>
    {
    public:
        typedef vecN<4> base;

        // Uninitialized variable
        inline vec4() {}

        // Copy constructor
        inline vec4(const base& v) : base(v) {}

        inline vec4(float scale)
            : vec4(scale, scale, scale, scale)
        {
        }

        // vec4(x, y, z, w);
        inline vec4(float x, float y, float z, float w)
        {
            base::data[0] = x;
            base::data[1] = y;
            base::data[2] = z;
            base::data[3] = w;
        }
    };

    template <int n>
    static inline const vecN<n> operator * (float x, const vecN<n>& v)
    {
        return v * x;
    }

    static inline const vec3 operator / (float x, const vec3& v)
    {
        return vec3(x / v[0], x / v[1], x / v[2]);
    }

    static inline const vec4 operator / (float x, const vec4& v)
    {
        return vec4(x / v[0], x / v[1], x / v[2], x / v[3]);
    }

    template <int len>
    static inline float dot(const vecN<len>& a, const vecN<len>& b)
    {
        float total = 0.0f;
        for (int n = 0; n < len; n++)
        {
            total += a[n] * b[n];
        }
        return total;
    }

    static inline vec3 cross(const vec3& a, const vec3& b)
    {
        return vec3(a[1] * b[2] - b[1] * a[2],
            a[2] * b[0] - b[2] * a[0],
            a[0] * b[1] - b[0] * a[1]);
    }

    template <int len>
    static inline float length(const vecN<len>& v)
    {
        float result = 0.0f;
        for (int i = 0; i < v.size(); ++i)
        {
            result += v[i] * v[i];
        }

        return sqrt(result);
    }

    template <int len>
    static inline vecN<len> normalize(const vecN<len>& v)
    {
        return v / length(v);
    }

    template <int len>
    static inline float distance(const vecN<len>& a, const vecN<len>& b)
    {
        return length(b - a);
    }

    template <int len>
    static inline float angle(const vecN<len>& a, const vecN<len>& b)
    {
        return arccos(dot(a, b));
    }

    class mat4
    {
    public:
        inline mat4()
        {
        }

        // Copy constructor
        inline mat4(const mat4& that)
        {
            assign(that);
        }

        inline mat4(const vec4& v0,
            const vec4& v1,
            const vec4& v2,
            const vec4& v3)
        {
            data[0] = v0;
            data[1] = v1;
            data[2] = v2;
            data[3] = v3;
        }

        // Construction from element type
        // explicit to prevent assignment from float
        explicit inline mat4(float f)
        {
            for (int n = 0; n < 4; n++)
            {
                data[n] = vmath::vec4(f, f, f, f);
            }
        }

        // Construction from vector
        inline mat4(const vec4& v)
        {
            for (int n = 0; n < 4; n++)
            {
                data[n] = v;
            }
        }

        // Assignment operator
        inline mat4& operator=(const mat4& that)
        {
            assign(that);
            return *this;
        }

        inline mat4 operator+(const mat4& that) const
        {
            mat4 result;
            for (int n = 0; n < 4; n++)
                result.data[n] = data[n] + that.data[n];
            return result;
        }

        inline mat4& operator+=(const mat4& that)
        {
            return (*this = *this + that);
        }

        inline mat4 operator-(const mat4& that) const
        {
            mat4 result;
            for (int n = 0; n < 4; n++)
                result.data[n] = data[n] - that.data[n];
            return result;
        }

        inline mat4& operator-=(const mat4& that)
        {
            return (*this = *this - that);
        }

        inline mat4 operator*(const float& that) const
        {
            mat4 result;
            for (int n = 0; n < 4; n++)
                result.data[n] = data[n] * that;
            return result;
        }

        inline mat4& operator*=(const float& that)
        {
            for (int n = 0; n < 4; n++)
                data[n] = data[n] * that;
            return *this;
        }

        // Matrix multiply.
        inline mat4 operator*(const mat4& that) const
        {
            mat4 result(0);

            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    float sum = 0.0f;
                    for (int n = 0; n < 4; n++)
                    {
                        sum += data[n][i] * that[j][n];
                    }

                    result[j][i] = sum;
                }
            }

            return result;
        }

        inline mat4& operator*=(const mat4& that)
        {
            return (*this = *this * that);
        }

        inline vec4& operator[](int n) { return data[n]; }
        inline const vec4& operator[](int n) const { return data[n]; }
        inline operator float*() { return &data[0][0]; }
        inline operator const float*() const { return &data[0][0]; }

        inline mat4 transpose(void) const
        {
            mat4 result;
            for (int y = 0; y < 4; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    result[x][y] = data[y][x];
                }
            }

            return result;
        }

        static inline mat4 identity()
        {
            mat4 result(0);

            for (int i = 0; i < 4; i++)
            {
                result[i][i] = 1;
            }

            return result;
        }

    protected:
        // Column primary data (essentially, array of vectors)
        vec4 data[4];

        // Assignment function - called from assignment operator and copy constructor.
        inline void assign(const mat4& that)
        {
            for (int n = 0; n < 4; n++)
                data[n] = that.data[n];
        }
    };

    static inline mat4 perspective(float fovy, float aspect, float n, float f)
    {
        float q = 1.0f / tanf(radians(0.5f * fovy));
        float A = q / aspect;
        float B = (n + f) / (n - f);
        float C = (2.0f * n * f) / (n - f);

        mat4 result;

        result[0] = vec4(A, 0.0f, 0.0f, 0.0f);
        result[1] = vec4(0.0f, q, 0.0f, 0.0f);
        result[2] = vec4(0.0f, 0.0f, B, -1.0f);
        result[3] = vec4(0.0f, 0.0f, C, 0.0f);

        return result;
    }

    static inline mat4 translate(float x, float y, float z)
    {
        return mat4(
            vec4(1.0f, 0.0f, 0.0f, 0.0f),
            vec4(0.0f, 1.0f, 0.0f, 0.0f),
            vec4(0.0f, 0.0f, 1.0f, 0.0f),
            vec4(x, y, z, 1.0f));
    }

    static inline mat4 translate(const vec3& v)
    {
        return translate(v[0], v[1], v[2]);
    }

    static inline mat4 lookat(const vec3& eye, const vec3& center, const vec3& up)
    {
        const vec3 f = normalize(center - eye);
        const vec3 upN = normalize(up);
        const vec3 s = cross(f, upN);
        const vec3 u = cross(s, f);
        const mat4 M = mat4(
            vec4(s[0], u[0], -f[0], 0.0f),
            vec4(s[1], u[1], -f[1], 0.0f),
            vec4(s[2], u[2], -f[2], 0.0f),
            vec4(0.0f, 0.0f, 0.0f, 1.0f));

        return M * translate(-eye);
    }

    static inline mat4 scale(float x, float y, float z)
    {
        return mat4(
            vec4(x, 0.0f, 0.0f, 0.0f),
            vec4(0.0f, y, 0.0f, 0.0f),
            vec4(0.0f, 0.0f, z, 0.0f),
            vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }

    static inline mat4 scale(const vec3& v)
    {
        return scale(v[0], v[1], v[2]);
    }

    static inline mat4 rotate(float angle, float x, float y, float z)
    {
        mat4 result;

        const float x2 = x * x;
        const float y2 = y * y;
        const float z2 = z * z;
        float rads = float(angle) * 0.0174532925f;
        const float c = cosf(rads);
        const float s = sinf(rads);
        const float omc = 1.0f - c;

        result[0] = vec4(x2 * omc + c, y * x * omc + z * s, x * z * omc - y * s, 0.0f);
        result[1] = vec4(x * y * omc - z * s, y2 * omc + c, y * z * omc + x * s, 0.0f);
        result[2] = vec4(x * z * omc + y * s, y * z * omc - x * s, z2 * omc + c, 0.0f);
        result[3] = vec4(0.0f, 0.0f, 0.0f, 1.0f);

        return result;
    }

    static inline mat4 rotate(float angle, const vec3& v)
    {
        return rotate(angle, v[0], v[1], v[2]);
    }

    // Pulled from 'The Mesa 3-D graphics library', gluInvertMatrix, refactored to integrate here.
    // Computes the inverse of the provided 4x4 matrix. *Assumes the matrix is invertable*
    static inline void inverse(mat4& matrix, mat4& result)
    {
        // Reading directly from the matrix.
        float* m = &matrix[0][0];
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

    const vec3 DEFAULT_FORWARD_VECTOR = vec3(0, 0, -1.0f);
    const vec3 DEFAULT_UP_VECTOR = vec3(0, -1.0f, 0);
    const float NORMALIZE_TOLERANCE = 0.00001f;

    class quaternion
    {
    public:
        inline quaternion()
        {
        }

        // Assignment operator
        inline quaternion& operator=(const quaternion& that)
        {
            x = that.x;
            y = that.y;
            z = that.z;
            w = that.w;
            return *this;
        }

        inline quaternion(const quaternion& that)
        {
            x = that.x;
            y = that.y;
            z = that.z;
            w = that.w;
        }

        inline quaternion(float _x, float _y, float _z, float _w)
        {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }

        inline quaternion operator*(const quaternion& q) const
        {
            const float x2 = q.x;
            const float y2 = q.y;
            const float z2 = q.z;
            const float w2 = q.w;

            return quaternion(
                w * x2 + x * w2 + y * z2 - z * y2,
                w * y2 + y * w2 + z * x2 - x * z2,
                w * z2 + z * w2 + x * y2 - y * x2,
                w * w2 - x * x2 - y * y2 - z * z2);
        }

        inline void normalize()
        {
            float magnitude = x*x + y*y + z*z + w*w;
            if (fabsf(magnitude - 1) < NORMALIZE_TOLERANCE)
            {
                float magnitudeReal = sqrtf(magnitude);
                x /= magnitudeReal;
                y /= magnitudeReal;
                z /= magnitudeReal;
                w /= magnitudeReal;
            }
        }

        inline quaternion conjugate() const
        {
            return quaternion(-x, -y, -z, w);
        }

        // Given an axis (unit vector) and an angle (in radians), returns a unit quaternion.
        static inline quaternion fromAxisAngle(float angle, vec3 axis)
        {
            float halfAngle = angle * 0.5f;
            float sinAngle = sinf(halfAngle);

            float x = (axis[0] * sinAngle);
            float y = (axis[1] * sinAngle);
            float z = (axis[2] * sinAngle);
            float w = cosf(halfAngle);

            return quaternion(x, y, z, w);
        }

        inline vec3 upVector() const
        {
            quaternion resultingVector = *this * (quaternion(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2], 0) * this->conjugate());
            return vec3(resultingVector.x, resultingVector.y, resultingVector.z);
        }

        inline vec3 forwardVector() const
        {
            quaternion resultingVector = *this * (quaternion(DEFAULT_FORWARD_VECTOR[0], DEFAULT_FORWARD_VECTOR[1], DEFAULT_FORWARD_VECTOR[2], 0) * this->conjugate());
            return vec3(resultingVector.x, resultingVector.y, resultingVector.z);
        }

        // Returns the Yaw-then-Pitch-then-Roll XYZ Euler Angles from the quaternion, in radians.
        inline vec3 asEulerAngles() const
        {
            return vec3(
                atan2f(2 * (w*x + y*z), 1 - 2 * (x*x + y*y)),
                asinf(2 * (w*y - z*x)),
                atan2f(2 * (w*z + x*y), 1 - 2 * (y*y + z*z)));
        }

        inline mat4 asMatrix() const
        {
            mat4 m;

            const float xx = x * x;
            const float yy = y * y;
            const float zz = z * z;
            const float xy = x * y;
            const float xz = x * z;
            const float xw = x * w;
            const float yz = y * z;
            const float yw = y * w;
            const float zw = z * w;

            m[0][0] = 1.0f - 2.0f * (yy + zz);
            m[0][1] = 2.0f * (xy - zw);
            m[0][2] = 2.0f * (xz + yw);
            m[0][3] = 0.0f;

            m[1][0] = 2.0f * (xy + zw);
            m[1][1] = 1.0f - 2.0f * (xx + zz);
            m[1][2] = 2.0f * (yz - xw);
            m[1][3] = 0.0f;

            m[2][0] = 2.0f * (xz - yw);
            m[2][1] = 2.0f * (yz + xw);
            m[2][2] = 1.0f - 2.0f * (xx + yy);
            m[2][3] = 0.0f;

            m[3][0] = 0.0f;
            m[3][1] = 0.0f;
            m[3][2] = 0.0f;
            m[3][3] = 1.0f;

            return m;
        }

        float x;
        float y;
        float z;
        float w;
    };

#ifdef min
#undef min
#endif

    static inline float min(float a, float b)
    {
        return a < b ? a : b;
    }

#ifdef max
#undef max
#endif

    static inline float max(float a, float b)
    {
        return a >= b ? a : b;
    }

    static inline vec4 operator*(const vec4& vec, const mat4& mat)
    {
        vec4 result = vec4(0.0f);
        for (int m = 0; m < 4; m++)
        {
            for (int n = 0; n < 4; n++)
            {
                result[n] += vec[m] * mat[n][m];
            }
        }

        return result;
    }

    template <const int N>
    static inline vecN<N> operator/(const float s, const vecN<N>& v)
    {
        vecN<N> result;
        for (int n = 0; n < N; n++)
        {
            result[n] = s / v[n];
        }

        return result;
    }

    // Computes a ray from the current mouse position into the scene.
    static inline vec3 screenRay(vec2 mouse, vec2 screenSize, mat4& perspectiveMatrix, mat4& viewRotationMatrix)
    {
        // Scale from -1.0 to 1.0, and invert Y
        vec2 deviceCoords = (mouse * 2.0f - screenSize) / screenSize;
        deviceCoords[1] = -deviceCoords[1];

        // Point the ray away from us.
        vec4 clippedRay = vec4(deviceCoords[0], deviceCoords[1], -1.0f, 1.0f);

        // Invert our projection and use the normal view matrix to end up with a world ray, which is exactly what we want.
        mat4 invPerspectiveMatrix;
        inverse(perspectiveMatrix, invPerspectiveMatrix);
        vec4 eyeRay = clippedRay * invPerspectiveMatrix;
        eyeRay[2] = -1.0f;
        eyeRay[3] = 0.0f;

        vec4 worldRay = eyeRay * viewRotationMatrix;

        vec3 result = vec3(worldRay[0], worldRay[1], worldRay[2]);
        return normalize(result);
    }
};
