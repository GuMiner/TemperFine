#pragma once
#include <cmath>

// Holds basic vector, matrix and quaternion operations.
// For more complicated operations, see MathOps, MatrixOps, PhysicsOps, and VecOps.
namespace vec
{
    // 2-element vector. Supports float and int data types, using strong-template binding in the CPP.
    template <typename T>
    class vec2T
    {
    public:
        T x;
        T y;

        // Uninitialized
        vec2T() { }

        // Copy constructor
        vec2T(const vec2T& other);

        // Assignment operator
        vec2T& operator=(const vec2T& other);

        // Dual-value construction.
        vec2T(T x, T y);

        // Single-value construction.
        vec2T(T value);

        // Overridden +-= operators
        vec2T& operator+=(const vec2T& other);
        vec2T& operator-=(const vec2T& other);

        // Overridden +- operators
        vec2T operator+(const vec2T& other) const;
        vec2T operator-() const;
        vec2T operator-(const vec2T& other) const;

        // Overridden * operators
        vec2T operator*(const vec2T& other) const;
        vec2T operator*(const T& other) const;

        // Overridden *= operators
        vec2T& operator*=(const vec2T& other);
        vec2T& operator*=(const T& other);

        // Overridden / operators.
        vec2T operator/(const vec2T& other) const;
        vec2T operator/(const T& other) const;

        // Overridden /= operators.
        vec2T& operator/=(const vec2T& other);
        vec2T& operator/=(const T& other);
    };

    typedef vec2T<float> vec2;
    typedef vec2T<int> vec2i;

    // 3-element vector. Supports float and int data types, using strong-template binding in the CPP.
    template <typename T>
    class vec3T
    {
    public:
        T x;
        T y;
        T z;

        // Uninitialized
        vec3T() { }

        // Copy constructor
        vec3T(const vec3T& other);

        // Assignment operator
        vec3T& operator=(const vec3T& other);

        // Triple-value construction.
        vec3T(T x, T y, T z);

        // Single-value construction.
        vec3T(T value);

        // Direct-access operators. Dangerous, but useful for high-speed operations.
        // Not stored in the C++ file as this is specific to vecT items.
        inline T& operator[](int n) { return *(&x + n); }
        inline const T& operator[](int n) const { return *(&x + n); }

        // Overridden +-= operators
        vec3T& operator+=(const vec3T& other);
        vec3T& operator-=(const vec3T& other);

        // Overridden +- operators
        vec3T operator+(const vec3T& other) const;
        vec3T operator-() const;
        vec3T operator-(const vec3T& other) const;

        // Overridden * operators
        vec3T operator*(const vec3T& other) const;
        vec3T operator*(const T& other) const;

        // Overridden *= operators
        vec3T& operator*=(const vec3T& other);
        vec3T& operator*=(const T& other);

        // Overridden / operators.
        vec3T operator/(const vec3T& other) const;
        vec3T operator/(const T& other) const;

        // Overridden /= operators.
        vec3T& operator/=(const vec3T& other);
        vec3T& operator/=(const T& other);
    };

    typedef vec3T<float> vec3;
    typedef vec3T<int> vec3i;

    // Comparer for the integer variant of vec3T, to support adding in std sets and maps.
    class vec3iComparer
    {
    public:
        // Compares two vec3i objects for storage comparisons.
        // If this returns false for comp(a, b) and comp(b, a), the objects are equal.
        bool operator()(const vec3i& lhs, const vec3i& rhs) const
        {
            // TODO this is really wrong (assuming max voxel size), but also right (anything that big wouldn't run.
            const int maxVoxelSize = 10000;
            return (lhs.x + lhs.y * maxVoxelSize + lhs.z * maxVoxelSize * maxVoxelSize) > (rhs.x + rhs.y * maxVoxelSize + rhs.z * maxVoxelSize * maxVoxelSize);
        }
    };

    // 4-element vector. Supports float and int data types, using strong-template binding in the CPP.
    template <typename T>
    class vec4T
    {
    public:
    public:
        T x;
        T y;
        T z;
        T w;

        // Uninitialized
        vec4T() { }

        // Copy constructor
        vec4T(const vec4T& other);

        // Assignment operator
        vec4T& operator=(const vec4T& other);

        // Quad-value construction.
        vec4T(T x, T y, T z, T w);

        // Single-value construction.
        vec4T(T value);

        // Direct-access operators. Dangerous, but useful for high-speed operations.
        // Not stored in the C++ file as this is specific to vecT items.
        inline T& operator[](int n) { return *(&x + n); }
        inline const T& operator[](int n) const { return *(&x + n); }

        // Overridden +-= operators
        vec4T& operator+=(const vec4T& other);
        vec4T& operator-=(const vec4T& other);

        // Overridden +- operators
        vec4T operator+(const vec4T& other) const;
        vec4T operator-() const;
        vec4T operator-(const vec4T& other) const;

        // Overridden * operators
        vec4T operator*(const vec4T& other) const;
        vec4T operator*(const T& other) const;

        // Overridden *= operators
        vec4T& operator*=(const vec4T& other);
        vec4T& operator*=(const T& other);

        // Overridden / operators.
        vec4T operator/(const vec4T& other) const;
        vec4T operator/(const T& other) const;

        // Overridden /= operators.
        vec4T& operator/=(const vec4T& other);
        vec4T& operator/=(const T& other);
    };

    typedef vec4T<float> vec4;
    typedef vec4T<int> vec4i;

    // Multiplication of T * vector.
    template <typename T>
    static inline const vec4T<T> operator*(T x, const vec4T<T>& v)
    {
        return v * x;
    }

    template <typename T>
    static inline const vec3T<T> operator*(T x, const vec3T<T>& v)
    {
        return v * x;
    }

    // Division of T / vector.
    template <typename T>
    static inline const vec3T<T> operator / (T x, const vec3T<T>& v)
    {
        return vec3T<T>(x / v.x, x / v.y, x / v.z);
    }

    template <typename T>
    static inline const vec4T<T>  operator / (T x, const vec4T<T>& v)
    {
        return vec4T<T>(x / v.x, x / v.y, x / v.z, x / v.w);
    }

    // Length and normalization, only really useful for 3-element vectors.
    template <typename T>
    static inline T length(const vec3T<T>& vector)
    {
        return sqrt(vector.x*vector.x + vector.y*vector.y + vector.z*vector.z);
    }

    template <typename T>
    static inline vec3T<T> normalize(const vec3T<T>& v)
    {
        return v / length(v);
    }

    // 4x4 matrix. Uses 4 vec4 vectors.
    class mat4
    {
    public:
        mat4()
        {
        }

        // Copy constructor
        mat4(const mat4& other);

        mat4(const vec4& v0,
             const vec4& v1,
             const vec4& v2,
             const vec4& v3);

        // Construction from vector
        mat4(const vec4& v);

        // Assignment operator
        mat4& operator=(const mat4& other);

        // Addition and substraction with other matrixes.
        mat4 operator+(const mat4& other) const;
        mat4& operator+=(const mat4& other);
        mat4 operator-(const mat4& other) const;
        mat4& operator-=(const mat4& other);

        // Multiplication of the matrix by a value
        mat4 operator*(const float& other) const;
        mat4& operator*=(const float& other);

        // Matrix multiplication.
        mat4 operator*(const mat4& other) const;
        mat4& operator*=(const mat4& other);

        // Direct data access operators.
        inline vec4& operator[](int n) { return data[n]; }
        inline const vec4& operator[](int n) const { return data[n]; }
        inline operator float*() { return &data[0][0]; }
        inline operator const float*() const { return &data[0][0]; }

        // Returns the transpose of this matrix.
        mat4 transpose(void) const;

        // Returns the identity matrix.
        static mat4 identity();

    private:
        // Column primary data (essentially, array of vectors)
        vec4 data[4];
    };

    // Matrix-Vector multiplication. Results in a vector.
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

    const vec3 DEFAULT_FORWARD_VECTOR = vec3(0, 0, -1.0f);
    const vec3 DEFAULT_UP_VECTOR = vec3(0, -1.0f, 0);
    const float NORMALIZE_TOLERANCE = 0.00001f;

    // Quaternion. Effectively a vec4, but with different manipulation and interaction syntax.
    class quaternion
    {
    public:
        float x;
        float y;
        float z;
        float w;

        quaternion()
        {
        }

        // Assignment operator
        quaternion& operator=(const quaternion& other);

        // Copy operator
        quaternion(const quaternion& other);

        // Quad-assignment setup.
        quaternion(float x, float y, float z, float w);

        // Performs special quaternion multiplication.
        quaternion operator*(const quaternion& q) const;

        // Normalizes this vector
        void normalize();

        // Returns the quaternion conjugate.
        quaternion conjugate() const;

        // Given an axis (unit vector) and an angle (in radians), returns a unit quaternion.
        static quaternion fromAxisAngle(float angle, vec3 axis);

        // Returns the up vector, given the current quaternion rotation.
        vec3 upVector() const;

        // Returns the forward vector, given the current quaternion rotation.
        vec3 forwardVector() const;

        // Returns the Yaw-then-Pitch-then-Roll XYZ Euler Angles from the quaternion, in radians.
        vec3 asEulerAngles() const;

        // Returns the quaternion as a rotation matrix.
        mat4 asMatrix() const;
    };
};
