#include "Vec.h"

namespace vec
{
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
}
