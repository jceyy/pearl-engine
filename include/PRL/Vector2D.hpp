#ifndef VECTOR_2D_HPP
#define VECTOR_2D_HPP

#include <iostream>
#include "Types.hpp"

class Vector2D{
public:
    Vector2D();
    Vector2D(PosType x, PosType y);

    inline PosType norm() const;
    inline PosType norm2() const;
    inline void normalize();
    inline void zero();
    inline void set(PosType x, PosType y);
    inline bool nonzero();
    inline Vector2D& add(const Vector2D& vec);
    inline Vector2D& subtract(const Vector2D& vec);
    inline Vector2D& multiplyComponentwise(const Vector2D& vec);
    inline Vector2D& divideComponentwise(const Vector2D& vec);
    inline PosType crossProduct(const Vector2D& vec) const;
    inline PosType dotProduct(const Vector2D& vec) const;
    Vector2D& operator+=(Vector2D& vec);
    Vector2D& operator-=(Vector2D& vec);
    Vector2D& operator*=(Vector2D& vec);
    Vector2D& operator/=(Vector2D& vec);
    Vector2D& operator+=(PosType a);
    Vector2D& operator-=(PosType a);
    Vector2D& operator*=(PosType a);
    Vector2D& operator/=(PosType a);
    
    static PosType dotProduct(const Vector2D& v1, const Vector2D& v2);
    static PosType crossProduct(const Vector2D& v1, const Vector2D& v2);

    PosType x;
    PosType y;
};

#include "Vector2D.inl"
Vector2D& operator-(Vector2D& vec);
Vector2D& operator*(PosType a, const Vector2D& vec);

Vector2D& operator+(const Vector2D& v1, const Vector2D& v2);
Vector2D& operator-(const Vector2D& v1, const Vector2D& v2);
Vector2D& operator*(const Vector2D& v1, const Vector2D& v2);
Vector2D& operator/(const Vector2D& v1, const Vector2D& v2);
Vector2D& operator+(const Vector2D& v1, PosType a);
Vector2D& operator-(const Vector2D& v1, PosType a);
Vector2D& operator*(const Vector2D& v1, PosType a);
Vector2D& operator/(const Vector2D& v1, PosType a);
std::ostream& operator<<(std::ostream& os, const Vector2D& vec);

#endif // VECTOR_2D_HPP