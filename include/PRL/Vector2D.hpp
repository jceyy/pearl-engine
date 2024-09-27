#ifndef VECTOR_2D_HPP
#define VECTOR_2D_HPP

#include <iostream>
#include "Types.hpp"

class Vector2D{
public:
    Vector2D();
    Vector2D(float x, float y);

    float norm() const;
    float norm2() const;
    void normalize();
    void zero();
    void set(float x, float y);
    bool nonzero();
    
    Vector2D& add(const Vector2D& vec);
    Vector2D& subtract(const Vector2D& vec);
    Vector2D& multiply(const Vector2D& vec);
    Vector2D& divide(const Vector2D& vec);
    float crossProduct(const Vector2D& vec) const;
    float dotProduct(const Vector2D& vec) const;
    static float dotProduct(const Vector2D& v1, const Vector2D& v2);
    static float crossProduct(const Vector2D& v1, const Vector2D& v2);
    Vector2D& operator+=(Vector2D& vec);
    Vector2D& operator-=(Vector2D& vec);
    Vector2D& operator*=(Vector2D& vec);
    Vector2D& operator/=(Vector2D& vec);
    Vector2D& operator+=(float a);
    Vector2D& operator-=(float a);
    Vector2D& operator*=(float a);
    Vector2D& operator/=(float a);

    PosType x;
    PosType y;
};

Vector2D& operator-(Vector2D& vec);
Vector2D& operator*(float a, const Vector2D& vec);

Vector2D& operator+(const Vector2D& v1, const Vector2D& v2);
Vector2D& operator-(const Vector2D& v1, const Vector2D& v2);
Vector2D& operator*(const Vector2D& v1, const Vector2D& v2);
Vector2D& operator/(const Vector2D& v1, const Vector2D& v2);
Vector2D& operator+(const Vector2D& v1, float a);
Vector2D& operator-(const Vector2D& v1, float a);
Vector2D& operator*(const Vector2D& v1, float a);
Vector2D& operator/(const Vector2D& v1, float a);
std::ostream& operator<<(std::ostream& os, const Vector2D& vec);

#endif // VECTOR_2D_HPP