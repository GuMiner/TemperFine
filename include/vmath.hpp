#pragma once
#include <cmath>

// From the OpenGL Superbible, 6th Ed, 'free to use in your application' as per the website.
// Modified so that quaternions can actually be used and removing dead code.
namespace vmath
{
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

    // Same with this tiny guy.
    class vec2i
    {
    public:
        int x;
        int y;
    };

    // This little guy exists until I get around to refactoring the math library correctly.
    class vec3i
    {
    public:

        vec3i()
        {
            x = 0;
            y = 0;
            z = 0;
        }

        vec3i(int x, int y, int z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        int x;
        int y;
        int z;
    };

    class vec3iComparer
    {
    public:
        // Compares two vec3i objects for storage comparisons.
        // If this returns false for comp(a, b) and comp(b, a), the objects are equal.
        bool operator()(vec3i const& lhs, vec3i const& rhs) const
        {
            // TODO this is really wrong (assuming max voxel size), but also right (anything that big wouldn't run.
            const int maxVoxelSize = 10000;
            return (lhs.x + lhs.y * maxVoxelSize + lhs.z * maxVoxelSize * maxVoxelSize) > (rhs.x + rhs.y * maxVoxelSize + rhs.z * maxVoxelSize * maxVoxelSize);
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
};
