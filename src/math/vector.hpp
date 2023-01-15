#ifndef MESHTOOL_VECTOR
#define MESHTOOL_VECTOR

#include <math.h>
#include <iostream>
#include <cassert>

// declarations

template<typename T>
struct Vector3
{
    public:
        Vector3();
        Vector3(const T &x, const T &y, const T &z);

    public:
        T x{};
        T y{};
        T z{};
};

template<typename T>
struct Vector2
{
    public:
        Vector2();
        Vector2(const T &x, const T &y);

    public:
        T x{};
        T y{};
};

// definitions

template<typename T>
Vector3<T>::Vector3() : x(0.0), y(0.0), z(0.0)
{}

template<typename T>
Vector3<T>::Vector3(const T &x, const T &y, const T &z) : x(x), y(y), z(z)
{}

template<typename T>
T dot(const Vector3<T> &vector1, const Vector3<T> &vector2)
{
    return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}

template<typename T>
Vector3<T> cross(const Vector3<T> &vector1, const Vector3<T> &vector2)
{
    return Vector3<T>(
        (vector1.y * vector2.z) - (vector1.z * vector2.y),
        (vector1.z * vector2.x) - (vector1.x * vector2.z),
        (vector1.x * vector2.y) - (vector1.y * vector2.x)
    );
}

template<typename T>
T tan_2D(const Vector3<T> &vector1, const Vector3<T> &vector2, const Vector3<T> &axis)
{
    T dot_product = dot(vector1, vector2);
    assert(dot_product != 0.0 && "division by 0 (90 degrees angle)");
    Vector3<T> cross_product = cross(vector1, vector2);
    T sign = dot(cross_product, axis) >= 0 ? 1.0 : -1.0;

    return sign * (length(cross_product) / dot_product);
}

template<typename T>
T cotan(const Vector3<T> &vector1, const Vector3<T> &vector2)
{
    T cross_length = length(cross(vector1, vector2));
    assert(cross_length != 0.0 && "division by 0 (colinear vectors)");
    return dot(vector1, vector2) / cross_length;
}

template<typename T>
bool is_near(const Vector3<T> &vector1, const Vector3<T> &vector2)
{
    return 
        abs(vector1.x - vector2.x) <= 0.0001 && 
        abs(vector1.y - vector2.y) <= 0.0001 && 
        abs(vector1.z - vector2.z) <= 0.0001;
}

template<typename T>
T length(const Vector3<T> &vector)
{
    return sqrt(length2(vector));
}

template<typename T>
T length2(const Vector3<T> &vector)
{
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

template<typename T>
Vector3<T> normalize(const Vector3<T> &vector)
{
    T len = length(vector);
    assert(len != 0.0 && "division by 0 (length is null)");
    return Vector3<T>(
        vector.x / len,
        vector.y / len,
        vector.z / len
    );
}

template<typename T>
Vector3<float> to_float(const Vector3<T> &vector)
{
    return Vector3<float>(float(vector.x), float(vector.y), float(vector.z));
}

template<typename T>
Vector3<T> operator+(const Vector3<T> &vector1, const Vector3<T> &vector2)
{
    return Vector3<T>(
        vector1.x + vector2.x,
        vector1.y + vector2.y,
        vector1.z + vector2.z
    );
}

template<typename T>
Vector3<T> &operator+=(Vector3<T> &vector1, const Vector3<T> &vector2)
{
    vector1.x += vector2.x;
    vector1.y += vector2.y;
    vector1.z += vector2.z;
    return vector1;
}
template<typename T>
Vector3<T> operator-(const Vector3<T> &vector1, const Vector3<T> &vector2)
{
    return Vector3<T>(
        vector1.x - vector2.x,
        vector1.y - vector2.y,
        vector1.z - vector2.z
    );
}

template<typename T>
Vector3<T> operator-(const Vector3<T> &vector)
{
    return Vector3<T>(
        -vector.x,
        -vector.y,
        -vector.z
    );
}

template<typename T>
Vector3<T> operator+(const Vector3<T> &vector, T value)
{
    return Vector3<T>(
        vector.x + value,
        vector.y + value,
        vector.z + value
    );
}

template<typename T>
Vector3<T> operator*(const Vector3<T> &vector, T value)
{
    return Vector3<T>(
        vector.x * value,
        vector.y * value,
        vector.z * value
    );
}

template<typename T>
Vector3<T> operator*(const Vector3<T> &vector, int value)
{
    return Vector3<T>(
        vector.x * value,
        vector.y * value,
        vector.z * value
    );
}

template<typename T>
Vector3<T> operator/(const Vector3<T> &vector, T value)
{
    assert(value != 0.0 && "division by 0");
    return Vector3<T>(
        vector.x / value,
        vector.y / value,
        vector.z / value
    );
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const Vector3<T> &vector)
{
    os << vector.x << " " << vector.y << " " << vector.z;
    return os;
}

template<typename T>
Vector2<T>::Vector2(const T &x, const T &y) : x(x), y(y)
{}

template<typename T>
bool is_near(const Vector2<T> &vector1, const Vector2<T> &vector2)
{
    return 
        abs(vector1.x - vector2.x) <= 0.0001 && 
        abs(vector1.y - vector2.y) <= 0.0001;
}

template<typename T>
T length(const Vector2<T> &vector)
{
    return sqrt(length2(vector));
}

template<typename T>
T length2(const Vector2<T> &vector)
{
    return vector.x * vector.x + vector.y * vector.y;
}

template<typename T>
Vector2<T> operator-(const Vector2<T> &vector1, const Vector2<T> &vector2)
{
    return Vector2<T>(
        vector1.x - vector2.x,
        vector1.y - vector2.y
    );
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const Vector2<T> &vector)
{
    os << vector.x << " " << vector.y;
    return os;
}

#endif