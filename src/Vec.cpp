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
}
