#include "Vector2D.hpp"
#include <cmath>

Vector2D::Vector2D() : x(0), y(0) {}

Vector2D::Vector2D(PosType x_, PosType y_) : x(x_), y(y_) {}

PosType Vector2D::dotProduct(const Vector2D& v1, const Vector2D& v2) {
    return v1.dotProduct(v2);
}

PosType Vector2D::crossProduct(const Vector2D& v1, const Vector2D& v2) {
    return v1.x*v2.y - v1.y*v2.x;
}
Vector2D& Vector2D::operator+=(Vector2D& vec) {
    return this->add(vec);
}

Vector2D& Vector2D::operator-=(Vector2D& vec) {
    return this->subtract(vec);
}

Vector2D& Vector2D::operator*=(Vector2D& vec) {
    return this->multiplyComponentwise(vec);
}

Vector2D& Vector2D::operator/=(Vector2D& vec) {
    return this->divideComponentwise(vec);
}

Vector2D& Vector2D::operator+=(PosType a) {
    x += a; y += a;
    return *this;
}

Vector2D& Vector2D::operator-=(PosType a) {
    x -= a; y -= a;
    return *this;
}

Vector2D& Vector2D::operator*=(PosType a) {
    x *= a; y *= a;
    return *this;
}

Vector2D& Vector2D::operator/=(PosType a) {
    x /= a; y /= a;
    return *this;
}


Vector2D& operator+(const Vector2D& v1, const Vector2D& v2) {
    Vector2D* v = new Vector2D(v1);
    return v->add(v2);
}

Vector2D& operator-(const Vector2D& v1, const Vector2D& v2) {
    Vector2D* v = new Vector2D(v1);
    return v->subtract(v2);
}

Vector2D& operator*(const Vector2D& v1, const Vector2D& v2) {
    Vector2D* v = new Vector2D(v1);
    return v->multiplyComponentwise(v2);
}

Vector2D& operator/(const Vector2D& v1, const Vector2D& v2) {
    Vector2D* v = new Vector2D(v1);
    return v->divideComponentwise(v2);
}

Vector2D& operator+(const Vector2D& v1, PosType a) {
    Vector2D* v = new Vector2D(v1);
    return *v += a;
}
Vector2D& operator-(const Vector2D& v1, PosType a) {
    Vector2D* v = new Vector2D(v1);
    return *v -= a;
}
Vector2D& operator*(PosType a, const Vector2D& vec) {
    Vector2D* v = new Vector2D(vec);
    return *v *= a;
}
Vector2D& operator*(const Vector2D& v1, PosType a) {
    Vector2D* v = new Vector2D(v1);
    return *v *= a;
}
Vector2D& operator/(const Vector2D& v1, PosType a) {
    Vector2D* v = new Vector2D(v1);
    return *v /= a;
}

Vector2D& operator-(Vector2D& vec) {
    return vec -= -1;
}

std::ostream& operator<<(std::ostream& os, const Vector2D& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}
