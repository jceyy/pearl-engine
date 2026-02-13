#ifndef VECTOR_2D_INL
#define VECTOR_2D_INL

#include "Vector2D.hpp"

inline PosType Vector2D::norm() const {
    return sqrt(x*x + y*y);
}
inline PosType Vector2D::norm2() const {
    return x*x + y*y;
}
inline void Vector2D::normalize() { 
    PosType a(norm()); 
    if (a != static_cast<PosType>(0)) *this /= a; 
}
inline void Vector2D::zero() { x = y = static_cast<PosType>(0); }
inline void Vector2D::set(PosType x, PosType y) { this->x = x; this->y = y; }
inline bool Vector2D::nonzero() { 
    return x != static_cast<PosType>(0) || y != static_cast<PosType>(0); 
}

inline Vector2D& Vector2D::add(const Vector2D& vec) {
    this->x += vec.x;
    this->y += vec.y;
    return *this;

}

inline Vector2D& Vector2D::subtract(const Vector2D& vec) {
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

inline Vector2D& Vector2D::multiplyComponentwise(const Vector2D& vec) {
    this->x *= vec.x;
    this->y *= vec.y;
    return *this;
}

inline Vector2D& Vector2D::divideComponentwise(const Vector2D& vec) {
    this->x /= vec.x;
    this->y /= vec.y;
    return *this;
}

inline PosType Vector2D::crossProduct(const Vector2D& vec) const {
    return x*vec.y - y*vec.x;
}

inline PosType Vector2D::dotProduct(const Vector2D& vec) const {
    return x*vec.x + y*vec.y;
}


#endif // VECTOR_2D_INL