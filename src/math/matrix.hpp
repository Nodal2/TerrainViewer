#ifndef MESHTOOL_MATRIX
#define MESHTOOL_MATRIX

#include <iostream>
#include <math.h>
#include <array>

#include "vector.hpp"

// declarations

template<typename T>
class Matrix4
{  
    public:
        Matrix4();
        Matrix4(   
                    T v00, T v01, T v02, T v03,
                    T v10, T v11, T v12, T v13,
                    T v20, T v21, T v22, T v23,
                    T v30, T v31, T v32, T v33
                );
        
        Matrix4(const std::array<T, 16> &m_row_major);

        Vector3<T> operator()(const Vector3<T> &vector) const;
        Vector3<T> operator[](unsigned int column_index) const;

    public:
        T m[4][4]{};
};

template<typename T>
Matrix4<T> Identity();

template<typename T>
Matrix4<T> UpperLeft(const Matrix4<T> &matrix);

template<typename T>
Matrix4<T> Scaling(const Vector3<T> &scaling);

template<typename T>
Matrix4<T> Translation(const Vector3<T> &translation);

template<typename T>
Matrix4<T> RotationX(T angle);

template<typename T>
Matrix4<T> RotationY(T angle);

template<typename T>
Matrix4<T> RotationZ(T angle);

template<typename T>
Matrix4<T> Rotation(const Vector3<T> &angles);

template<typename T>
Matrix4<T> Projection(T fov, T aspect_ratio, T z_near, T z_far);

template<typename T>
Matrix4<T> View(const Vector3<T> &direction, const Vector3<T> &up, const Vector3<T> &position);

template<typename T>
Matrix4<T> operator*(const Matrix4<T> &m1, const Matrix4<T> &m2);

template<typename T>
Matrix4<T> operator*(T value, const Matrix4<T> &m);

template<typename T>
std::ostream &operator<<(std::ostream &os, const Matrix4<T> &m);

// definitions

template<typename T>
Matrix4<T>::Matrix4()
{
    *this = Identity<T>();
}

template<typename T>
Matrix4<T>::Matrix4(   
                    T v00, T v01, T v02, T v03,
                    T v10, T v11, T v12, T v13,
                    T v20, T v21, T v22, T v23,
                    T v30, T v31, T v32, T v33
                )
{
    m[0][0] = v00; m[0][1] = v01; m[0][2] = v02; m[0][3] = v03;
    m[1][0] = v10; m[1][1] = v11; m[1][2] = v12; m[1][3] = v13;
    m[2][0] = v20; m[2][1] = v21; m[2][2] = v22; m[2][3] = v23;
    m[3][0] = v30; m[3][1] = v31; m[3][2] = v32; m[3][3] = v33;
}

template<typename T>
Matrix4<T>::Matrix4(const std::array<T, 16> &m_row_major)
{
    for(unsigned int i = 0; i < 4; ++i)
    {
        m[i][0] = m_row_major[4 * i];
        m[i][1] = m_row_major[4 * i + 1];
        m[i][2] = m_row_major[4 * i + 2];
        m[i][3] = m_row_major[4 * i + 3];
    }
}

template<typename T>
Vector3<T> Matrix4<T>::operator()(const Vector3<T> &vector) const
{
    Vector3<T> result;
    result.x = vector.x * m[0][0] + vector.y * m[0][1] + vector.z * m[0][2] + 1.0 * m[0][3];
    result.y = vector.x * m[1][0] + vector.y * m[1][1] + vector.z * m[1][2] + 1.0 * m[1][3];
    result.z = vector.x * m[2][0] + vector.y * m[2][1] + vector.z * m[2][2] + 1.0 * m[2][3];
    return result;
}

template<typename T>
Vector3<T> Matrix4<T>::operator[](unsigned int column_index) const
{
    assert(column_index < 4 && "column index out of bounds");
    return Vector3<float>(m[0][column_index], m[1][column_index], m[2][column_index]);
}

template<typename T>
Matrix4<T> Identity()
{
    return Matrix4<T>(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

template<typename T>
Matrix4<T> UpperLeft(const Matrix4<T> &matrix)
{
    return Matrix4<T>(
        matrix.m[0][0], matrix.m[0][1], matrix.m[0][2], 0.0,
        matrix.m[1][0], matrix.m[1][1], matrix.m[1][2], 0.0,
        matrix.m[2][0], matrix.m[2][1], matrix.m[2][2], 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

template<typename T>
Matrix4<T> Scaling(const Vector3<T> &scaling)
{
    return Matrix4<T>(
        scaling.x, 0.0, 0.0, 0.0,
        0.0, scaling.y, 0.0, 0.0,
        0.0, 0.0, scaling.z, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

template<typename T>
Matrix4<T> Translation(const Vector3<T> &translation)
{
    return Matrix4<T>(
        1.0, 0.0, 0.0, translation.x,
        0.0, 1.0, 0.0, translation.y,
        0.0, 0.0, 1.0, translation.z,
        0.0, 0.0, 0.0, 1.0
    );
}

template<typename T>
Matrix4<T> RotationX(T angle)
{
    return Matrix4<T>(
        1.0, 0.0, 0.0, 0.0,
        0.0, cos(angle), -sin(angle), 0.0,
        0.0, sin(angle), cos(angle), 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

template<typename T>
Matrix4<T> RotationY(T angle)
{
    return Matrix4<T>(
        cos(angle), 0.0, -sin(angle), 0.0,
        0.0, 1.0, 0.0, 0.0,
        sin(angle), 0.0, cos(angle), 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

template<typename T>
Matrix4<T> RotationZ(T angle)
{
    return Matrix4<T>(
        cos(angle), -sin(angle), 0.0, 0.0,
        sin(angle), cos(angle), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}

template<typename T>
Matrix4<T> Rotation(const Vector3<T> &angles)
{
    return RotationZ(angles.z) * RotationY(angles.y) * RotationX(angles.x);
}

template<typename T>
Matrix4<T> Projection(T fov, T aspect_ratio, T z_near, T z_far)
{
    assert(aspect_ratio != 0.0 && "aspect ratio is null");
    assert(z_near < z_far && "z_near >= z_far");

    T t = 1.0 / tan(fov);
    T z_range = z_near - z_far;

    T a = (-z_far - z_near) / z_range;
    T b = 2.0 * z_far * z_near / z_range;
    
    return Matrix4<T>(
        t/aspect_ratio, 0.0, 0.0, 0.0,
        0.0, t, 0.0, 0.0,
        0.0, 0.0, a, b,
        0.0, 0.0, 1.0, 0.0
    );
}

template<typename T>
Matrix4<T> View(const Vector3<T> &direction, const Vector3<T> &up, const Vector3<T> &position)
{
    Vector3<T> right_vector = normalize(cross(up, direction));
    Vector3<T> new_up = normalize(cross(direction, right_vector));
    Matrix4<T> camera_coords_matrix = Matrix4<T>(
        right_vector.x, right_vector.y, right_vector.z, 0.0,
        new_up.x, new_up.y, new_up.z, 0.0,
        direction.x, direction.y, direction.z, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    Matrix4<T> origin_translation_matrix = Matrix4<T>(
        1.0, 0.0, 0.0, -position.x,
        0.0, 1.0, 0.0, -position.y,
        0.0, 0.0, 1.0, -position.z,
        0.0, 0.0, 0.0, 1.0
    );
    
    return camera_coords_matrix * origin_translation_matrix;
}

template<typename T>
Matrix4<T> operator*(const Matrix4<T> &m1, const Matrix4<T> &m2)
{
    Matrix4<T> result;
    for(unsigned int i = 0; i < 4; ++i)
        for(unsigned int j = 0; j < 4; ++j)
            result.m[i][j] =    m1.m[i][0] * m2.m[0][j] +
                                m1.m[i][1] * m2.m[1][j] +
                                m1.m[i][2] * m2.m[2][j] +
                                m1.m[i][3] * m2.m[3][j];
    return result;
}

template<typename T>
Matrix4<T> operator*(T value, const Matrix4<T> &m)
{
    return Matrix4<T>(
        m.m[0][0] * value, m.m[0][1] * value, m.m[0][2] * value, m.m[0][3] * value,
        m.m[1][0] * value, m.m[1][1] * value, m.m[1][2] * value, m.m[1][3] * value,
        m.m[2][0] * value, m.m[2][1] * value, m.m[2][2] * value, m.m[2][3] * value,
        m.m[3][0] * value, m.m[3][1] * value, m.m[3][2] * value, m.m[3][3] * value
    );
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const Matrix4<T> &m)
{
    os << m.m[0][0] << "\t" << m.m[0][1] << "\t" << m.m[0][2] << "\t" << m.m[0][3] << "\n";
    os << m.m[1][0] << "\t" << m.m[1][1] << "\t" << m.m[1][2] << "\t" << m.m[1][3] << "\n";
    os << m.m[2][0] << "\t" << m.m[2][1] << "\t" << m.m[2][2] << "\t" << m.m[2][3] << "\n";
    os << m.m[3][0] << "\t" << m.m[3][1] << "\t" << m.m[3][2] << "\t" << m.m[3][3];
    return os;
}

#endif