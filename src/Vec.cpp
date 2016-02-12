#include "Vec.h"

namespace vec
{
    // --------------------------------------------------------------------
    // vec2T --------------------------------------------------------------

    // Copy constructor
    template<typename T>
    vec2T<T>::vec2T(const vec2T<T>& other)
    {
        x = other.x;
        y = other.y;
    }

    // Assignment operator
    template<typename T>
    vec2T<T>& vec2T<T>::operator=(const vec2T<T>& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

    // Dual-value construction.
    template<typename T>
    vec2T<T>::vec2T(T x, T y)
    {
        this->x = x;
        this->y = y;
    }

    // Single-value construction.
    template<typename T>
    vec2T<T>::vec2T(T value)
    {
        x = value;
        y = value;
    }

    // Overriden +-= operators
    template<typename T>
    vec2T<T>& vec2T<T>::operator+=(const vec2T<T>& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    template<typename T>
    vec2T<T>& vec2T<T>::operator-=(const vec2T<T>& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Overriden +- operators
    template<typename T>
    vec2T<T> vec2T<T>::operator+(const vec2T<T>& other) const
    {
        return vec2T<T>(x + other.x, y + other.y);
    }

    template<typename T>
    vec2T<T> vec2T<T>::operator-() const
    {
        return vec2T<T>(-x, -y);
    }

    template<typename T>
    vec2T<T> vec2T<T>::operator-(const vec2T<T>& other) const
    {
        return vec2T<T>(x - other.x, y - other.y);
    }

    // Overridden * operators
    template<typename T>
    vec2T<T> vec2T<T>::operator*(const vec2T<T>& other) const
    {
        return vec2T(x * other.x, y * other.y);
    }

    template<typename T>
    vec2T<T> vec2T<T>::operator*(const T& other) const
    {
        return vec2T<T>(x * other, y * other);
    }

    // Overridden *= operators
    template<typename T>
    vec2T<T>& vec2T<T>::operator*=(const vec2T<T>& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    template<typename T>
    vec2T<T>& vec2T<T>::operator*=(const T& other)
    {
        x *= other;
        y *= other;
        return *this;
    }

    // Overridden / operators.
    template<typename T>
    vec2T<T> vec2T<T>::operator/(const vec2T<T>& other) const
    {
        return vec2T<T>(x / other.x, y / other.y);
    }

    template<typename T>
    vec2T<T> vec2T<T>::operator/(const T& other) const
    {
        return vec2T<T>(x / other, y / other);
    }

    // Overridden /= operators.
    template<typename T>
    vec2T<T>& vec2T<T>::operator/=(const vec2T<T>& other)
    {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    template<typename T>
    vec2T<T>& vec2T<T>::operator/=(const T& other)
    {
        x /= other;
        y /= other;
        return *this;
    }

    // Strong template float and int definitions.

    // Copy constructor
    template vec2T<float>::vec2T(const vec2T<float>& other);
    template vec2T<int>::vec2T(const vec2T<int>& other);

    // Assignment operator
    template vec2T<float>& vec2T<float>::operator=(const vec2T<float>& other);
    template vec2T<int>& vec2T<int>::operator=(const vec2T<int>& other);

    // Dual-value construction.
    template vec2T<float>::vec2T(float x, float y);
    template vec2T<int>::vec2T(int x, int y);

    // Single-value construction.
    template vec2T<float>::vec2T(float value);
    template vec2T<int>::vec2T(int value);

    // Overriden +-= operators
    template vec2T<float>& vec2T<float>::operator+=(const vec2T<float>& other);
    template vec2T<float>& vec2T<float>::operator-=(const vec2T<float>& other);
    template vec2T<int>& vec2T<int>::operator+=(const vec2T<int>& other);
    template vec2T<int>& vec2T<int>::operator-=(const vec2T<int>& other);

    // Overriden +- operators
    template vec2T<float> vec2T<float>::operator+(const vec2T<float>& other) const;
    template vec2T<float> vec2T<float>::operator-() const;
    template vec2T<float> vec2T<float>::operator-(const vec2T<float>& other) const;
    template vec2T<int> vec2T<int>::operator+(const vec2T<int>& other) const;
    template vec2T<int> vec2T<int>::operator-() const;
    template vec2T<int> vec2T<int>::operator-(const vec2T<int>& other) const;

    // Overriden * operators
    template vec2T<float> vec2T<float>::operator*(const vec2T<float>& other) const;
    template vec2T<float> vec2T<float>::operator*(const float& other) const;
    template vec2T<int> vec2T<int>::operator*(const vec2T<int>& other) const;
    template vec2T<int> vec2T<int>::operator*(const int& other) const;

    // Overriden *= operators
    template vec2T<float>& vec2T<float>::operator*=(const vec2T<float>& other);
    template vec2T<float>& vec2T<float>::operator*=(const float& other);
    template vec2T<int>& vec2T<int>::operator*=(const vec2T<int>& other);
    template vec2T<int>& vec2T<int>::operator*=(const int& other);

    // Overriden / operators.
    template vec2T<float> vec2T<float>::operator/(const vec2T<float>& other) const;
    template vec2T<float> vec2T<float>::operator/(const float& other) const;
    template vec2T<int> vec2T<int>::operator/(const vec2T<int>& other) const;
    template vec2T<int> vec2T<int>::operator/(const int& other) const;

    // Overriden /= operators.
    template vec2T<float>& vec2T<float>::operator/=(const vec2T<float>& other);
    template vec2T<float>& vec2T<float>::operator/=(const float& other);
    template vec2T<int>& vec2T<int>::operator/=(const vec2T<int>& other);
    template vec2T<int>& vec2T<int>::operator/=(const int& other);

    // --------------------------------------------------------------------
    // vec3T --------------------------------------------------------------

    // Copy constructor
    template<typename T>
    vec3T<T>::vec3T(const vec3T<T>& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }

    // Assignment operator
    template<typename T>
    vec3T<T>& vec3T<T>::operator=(const vec3T<T>& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    // Triple-value construction.
    template<typename T>
    vec3T<T>::vec3T(T x, T y, T z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    // Single-value construction.
    template<typename T>
    vec3T<T>::vec3T(T value)
    {
        x = value;
        y = value;
        z = value;
    }

    // Overriden +-= operators
    template<typename T>
    vec3T<T>& vec3T<T>::operator+=(const vec3T<T>& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    template<typename T>
    vec3T<T>& vec3T<T>::operator-=(const vec3T<T>& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    // Overriden +- operators
    template<typename T>
    vec3T<T> vec3T<T>::operator+(const vec3T<T>& other) const
    {
        return vec3T<T>(x + other.x, y + other.y, z + other.z);
    }

    template<typename T>
    vec3T<T> vec3T<T>::operator-() const
    {
        return vec3T<T>(-x, -y, -z);
    }

    template<typename T>
    vec3T<T> vec3T<T>::operator-(const vec3T<T>& other) const
    {
        return vec3T<T>(x - other.x, y - other.y, z - other.z);
    }

    // Overridden * operators
    template<typename T>
    vec3T<T> vec3T<T>::operator*(const vec3T<T>& other) const
    {
        return vec3T(x * other.x, y * other.y, z * other.z);
    }

    template<typename T>
    vec3T<T> vec3T<T>::operator*(const T& other) const
    {
        return vec3T<T>(x * other, y * other, z * other);
    }

    // Overridden *= operators
    template<typename T>
    vec3T<T>& vec3T<T>::operator*=(const vec3T<T>& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    template<typename T>
    vec3T<T>& vec3T<T>::operator*=(const T& other)
    {
        x *= other;
        y *= other;
        z *= other;
        return *this;
    }

    // Overridden / operators.
    template<typename T>
    vec3T<T> vec3T<T>::operator/(const vec3T<T>& other) const
    {
        return vec3T<T>(x / other.x, y / other.y, z / other.z);
    }

    template<typename T>
    vec3T<T> vec3T<T>::operator/(const T& other) const
    {
        return vec3T<T>(x / other, y / other, z / other);
    }

    // Overridden /= operators.
    template<typename T>
    vec3T<T>& vec3T<T>::operator/=(const vec3T<T>& other)
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    template<typename T>
    vec3T<T>& vec3T<T>::operator/=(const T& other)
    {
        x /= other;
        y /= other;
        z /= other;
        return *this;
    }

    // Strong template float and int definitions.

    // Copy constructor
    template vec3T<float>::vec3T(const vec3T<float>& other);
    template vec3T<int>::vec3T(const vec3T<int>& other);

    // Assignment operator
    template vec3T<float>& vec3T<float>::operator=(const vec3T<float>& other);
    template vec3T<int>& vec3T<int>::operator=(const vec3T<int>& other);

    // Dual-value construction.
    template vec3T<float>::vec3T(float x, float y, float z);
    template vec3T<int>::vec3T(int x, int y, int z);

    // Single-value construction.
    template vec3T<float>::vec3T(float value);
    template vec3T<int>::vec3T(int value);

    // Overriden +-= operators
    template vec3T<float>& vec3T<float>::operator+=(const vec3T<float>& other);
    template vec3T<float>& vec3T<float>::operator-=(const vec3T<float>& other);
    template vec3T<int>& vec3T<int>::operator+=(const vec3T<int>& other);
    template vec3T<int>& vec3T<int>::operator-=(const vec3T<int>& other);

    // Overriden +- operators
    template vec3T<float> vec3T<float>::operator+(const vec3T<float>& other) const;
    template vec3T<float> vec3T<float>::operator-() const;
    template vec3T<float> vec3T<float>::operator-(const vec3T<float>& other) const;
    template vec3T<int> vec3T<int>::operator+(const vec3T<int>& other) const;
    template vec3T<int> vec3T<int>::operator-() const;
    template vec3T<int> vec3T<int>::operator-(const vec3T<int>& other) const;

    // Overriden * operators
    template vec3T<float> vec3T<float>::operator*(const vec3T<float>& other) const;
    template vec3T<float> vec3T<float>::operator*(const float& other) const;
    template vec3T<int> vec3T<int>::operator*(const vec3T<int>& other) const;
    template vec3T<int> vec3T<int>::operator*(const int& other) const;

    // Overriden *= operators
    template vec3T<float>& vec3T<float>::operator*=(const vec3T<float>& other);
    template vec3T<float>& vec3T<float>::operator*=(const float& other);
    template vec3T<int>& vec3T<int>::operator*=(const vec3T<int>& other);
    template vec3T<int>& vec3T<int>::operator*=(const int& other);

    // Overriden / operators.
    template vec3T<float> vec3T<float>::operator/(const vec3T<float>& other) const;
    template vec3T<float> vec3T<float>::operator/(const float& other) const;
    template vec3T<int> vec3T<int>::operator/(const vec3T<int>& other) const;
    template vec3T<int> vec3T<int>::operator/(const int& other) const;

    // Overriden /= operators.
    template vec3T<float>& vec3T<float>::operator/=(const vec3T<float>& other);
    template vec3T<float>& vec3T<float>::operator/=(const float& other);
    template vec3T<int>& vec3T<int>::operator/=(const vec3T<int>& other);
    template vec3T<int>& vec3T<int>::operator/=(const int& other);

    // --------------------------------------------------------------------
    // vec4T --------------------------------------------------------------

    // Copy constructor
    template<typename T>
    vec4T<T>::vec4T(const vec4T<T>& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }

    // Assignment operator
    template<typename T>
    vec4T<T>& vec4T<T>::operator=(const vec4T<T>& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }

    // Quad-value construction.
    template<typename T>
    vec4T<T>::vec4T(T x, T y, T z, T w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    // Single-value construction.
    template<typename T>
    vec4T<T>::vec4T(T value)
    {
        x = value;
        y = value;
        z = value;
        w = value;
    }

    // Overriden +-= operators
    template<typename T>
    vec4T<T>& vec4T<T>::operator+=(const vec4T<T>& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    template<typename T>
    vec4T<T>& vec4T<T>::operator-=(const vec4T<T>& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    // Overriden +- operators
    template<typename T>
    vec4T<T> vec4T<T>::operator+(const vec4T<T>& other) const
    {
        return vec4T<T>(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    template<typename T>
    vec4T<T> vec4T<T>::operator-() const
    {
        return vec4T<T>(-x, -y, -z, -w);
    }

    template<typename T>
    vec4T<T> vec4T<T>::operator-(const vec4T<T>& other) const
    {
        return vec4T<T>(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    // Overridden * operators
    template<typename T>
    vec4T<T> vec4T<T>::operator*(const vec4T<T>& other) const
    {
        return vec4T(x * other.x, y * other.y, z * other.z, w * other.w);
    }

    template<typename T>
    vec4T<T> vec4T<T>::operator*(const T& other) const
    {
        return vec4T<T>(x * other, y * other, z * other, w * other);
    }

    // Overridden *= operators
    template<typename T>
    vec4T<T>& vec4T<T>::operator*=(const vec4T<T>& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.z;
        return *this;
    }

    template<typename T>
    vec4T<T>& vec4T<T>::operator*=(const T& other)
    {
        x *= other;
        y *= other;
        z *= other;
        w *= other;
        return *this;
    }

    // Overridden / operators.
    template<typename T>
    vec4T<T> vec4T<T>::operator/(const vec4T<T>& other) const
    {
        return vec4T<T>(x / other.x, y / other.y, z / other.z, w / other.w);
    }

    template<typename T>
    vec4T<T> vec4T<T>::operator/(const T& other) const
    {
        return vec4T<T>(x / other, y / other, z / other, w / other);
    }

    // Overridden /= operators.
    template<typename T>
    vec4T<T>& vec4T<T>::operator/=(const vec4T<T>& other)
    {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;
        return *this;
    }

    template<typename T>
    vec4T<T>& vec4T<T>::operator/=(const T& other)
    {
        x /= other;
        y /= other;
        z /= other;
        w /= other;
        return *this;
    }

    // Strong template float and int definitions.

    // Copy constructor
    template vec4T<float>::vec4T(const vec4T<float>& other);
    template vec4T<int>::vec4T(const vec4T<int>& other);

    // Assignment operator
    template vec4T<float>& vec4T<float>::operator=(const vec4T<float>& other);
    template vec4T<int>& vec4T<int>::operator=(const vec4T<int>& other);

    // Dual-value construction.
    template vec4T<float>::vec4T(float x, float y, float z, float w);
    template vec4T<int>::vec4T(int x, int y, int z, int w);

    // Single-value construction.
    template vec4T<float>::vec4T(float value);
    template vec4T<int>::vec4T(int value);

    // Overriden +-= operators
    template vec4T<float>& vec4T<float>::operator+=(const vec4T<float>& other);
    template vec4T<float>& vec4T<float>::operator-=(const vec4T<float>& other);
    template vec4T<int>& vec4T<int>::operator+=(const vec4T<int>& other);
    template vec4T<int>& vec4T<int>::operator-=(const vec4T<int>& other);

    // Overriden +- operators
    template vec4T<float> vec4T<float>::operator+(const vec4T<float>& other) const;
    template vec4T<float> vec4T<float>::operator-() const;
    template vec4T<float> vec4T<float>::operator-(const vec4T<float>& other) const;
    template vec4T<int> vec4T<int>::operator+(const vec4T<int>& other) const;
    template vec4T<int> vec4T<int>::operator-() const;
    template vec4T<int> vec4T<int>::operator-(const vec4T<int>& other) const;

    // Overriden * operators
    template vec4T<float> vec4T<float>::operator*(const vec4T<float>& other) const;
    template vec4T<float> vec4T<float>::operator*(const float& other) const;
    template vec4T<int> vec4T<int>::operator*(const vec4T<int>& other) const;
    template vec4T<int> vec4T<int>::operator*(const int& other) const;

    // Overriden *= operators
    template vec4T<float>& vec4T<float>::operator*=(const vec4T<float>& other);
    template vec4T<float>& vec4T<float>::operator*=(const float& other);
    template vec4T<int>& vec4T<int>::operator*=(const vec4T<int>& other);
    template vec4T<int>& vec4T<int>::operator*=(const int& other);

    // Overriden / operators.
    template vec4T<float> vec4T<float>::operator/(const vec4T<float>& other) const;
    template vec4T<float> vec4T<float>::operator/(const float& other) const;
    template vec4T<int> vec4T<int>::operator/(const vec4T<int>& other) const;
    template vec4T<int> vec4T<int>::operator/(const int& other) const;

    // Overriden /= operators.
    template vec4T<float>& vec4T<float>::operator/=(const vec4T<float>& other);
    template vec4T<float>& vec4T<float>::operator/=(const float& other);
    template vec4T<int>& vec4T<int>::operator/=(const vec4T<int>& other);
    template vec4T<int>& vec4T<int>::operator/=(const int& other);

    // --------------------------------------------------------------------
    // mat4, non-templated ------------------------------------------------

    // Copy constructor
    mat4::mat4(const mat4& other)
    {
        for (unsigned int n = 0; n < 4; n++)
        {
            data[n] = other.data[n];
        }
    }

    mat4::mat4(const vec4& v0,
        const vec4& v1,
        const vec4& v2,
        const vec4& v3)
    {
        data[0] = v0;
        data[1] = v1;
        data[2] = v2;
        data[3] = v3;
    }

    // Construction from vector
    mat4::mat4(const vec4& v)
    {
        for (unsigned int n = 0; n < 4; n++)
        {
            data[n] = v;
        }
    }

    // Assignment operator
    mat4& mat4::operator=(const mat4& other)
    {
        for (unsigned int n = 0; n < 4; n++)
        {
            data[n] = other.data[n];
        }

        return *this;
    }

    mat4 mat4::operator+(const mat4& other) const
    {
        mat4 result;
        for (unsigned int n = 0; n < 4; n++)
        {
            result.data[n] = data[n] + other.data[n];
        }
        return result;
    }

    mat4& mat4::operator+=(const mat4& other)
    {
        return (*this = *this + other);
    }

    mat4 mat4::operator-(const mat4& other) const
    {
        mat4 result;
        for (unsigned int n = 0; n < 4; n++)
        {
            result.data[n] = data[n] - other.data[n];
        }
        return result;
    }

    mat4& mat4::operator-=(const mat4& other)
    {
        return (*this = *this - other);
    }

    mat4 mat4::operator*(const float& other) const
    {
        mat4 result;
        for (unsigned int n = 0; n < 4; n++)
        {
            result.data[n] = data[n] * other;
        }
        return result;
    }

    mat4& mat4::operator*=(const float& other)
    {
        for (unsigned int n = 0; n < 4; n++)
        {
            data[n] = data[n] * other;
        }
        return *this;
    }

    // Matrix multiply.
    mat4 mat4::operator*(const mat4& other) const
    {
        mat4 result(vec4(0, 0, 0, 0));

        for (int j = 0; j < 4; j++)
        {
            for (int i = 0; i < 4; i++)
            {
                float sum = 0.0f;
                for (int n = 0; n < 4; n++)
                {
                    sum += data[n][i] * other[j][n];
                }

                result[j][i] = sum;
            }
        }

        return result;
    }

    mat4& mat4::operator*=(const mat4& other)
    {
        return (*this = *this * other);
    }

    mat4 mat4::transpose(void) const
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

    mat4 mat4::identity()
    {
        mat4 result(vec4(0, 0, 0, 0));
        for (unsigned int i = 0; i < 4; i++)
        {
            result[i][i] = 1;
        }
        return result;
    }

    // --------------------------------------------------------------------
    // quaternion, non-templated ------------------------------------------

    // Assignment operator
    quaternion& quaternion::operator=(const quaternion& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }

    // Copy operator
    quaternion::quaternion(const quaternion& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }

    // Quad-assignment setup.
    quaternion::quaternion(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    // Performs special quaternion multiplication.
    quaternion quaternion::operator*(const quaternion& q) const
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

    // Normalizes this vector
    void quaternion::normalize()
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

    // Returns the quaternion conjugate.
    quaternion quaternion::conjugate() const
    {
        return quaternion(-x, -y, -z, w);
    }

    // Given an axis (unit vector) and an angle (in radians), returns a unit quaternion.
    quaternion quaternion::fromAxisAngle(float angle, vec3 axis)
    {
        float halfAngle = angle * 0.5f;
        float sinAngle = sinf(halfAngle);

        float x = (axis.x * sinAngle);
        float y = (axis.y * sinAngle);
        float z = (axis.z * sinAngle);
        float w = cosf(halfAngle);

        return quaternion(x, y, z, w);
    }

    // Returns the up vector, given the current quaternion rotation.
    vec3 quaternion::upVector() const
    {
        quaternion resultingVector = *this * (quaternion(DEFAULT_UP_VECTOR.x, DEFAULT_UP_VECTOR.y, DEFAULT_UP_VECTOR.z, 0) * this->conjugate());
        return vec3(resultingVector.x, resultingVector.y, resultingVector.z);
    }

    // Returns the forward vector, given the current quaternion rotation.
    vec3 quaternion::forwardVector() const
    {
        quaternion resultingVector = *this * (quaternion(DEFAULT_FORWARD_VECTOR.x, DEFAULT_FORWARD_VECTOR.y, DEFAULT_FORWARD_VECTOR.z, 0) * this->conjugate());
        return vec3(resultingVector.x, resultingVector.y, resultingVector.z);
    }

    // Returns the Yaw-then-Pitch-then-Roll XYZ Euler Angles from the quaternion, in radians.
    vec3 quaternion::asEulerAngles() const
    {
        return vec3(
            atan2f(2 * (w*x + y*z), 1 - 2 * (x*x + y*y)),
            asinf(2 * (w*y - z*x)),
            atan2f(2 * (w*z + x*y), 1 - 2 * (y*y + z*z)));
    }

    // Returns the quaternion as a rotation matrix.
    mat4 quaternion::asMatrix() const
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
}
