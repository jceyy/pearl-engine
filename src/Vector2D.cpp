#include "Vector2D.hpp"
#include <cmath>

Vector2D::Vector2D() : x(0), y(0) {}

Vector2D::Vector2D(float x_, float y_) : x(x_), y(y_) {}

Vector2D& Vector2D::add(const Vector2D& vec) {
    this->x += vec.x;
    this->y += vec.y;
    return *this;
}

Vector2D& Vector2D::subtract(const Vector2D& vec) {
    this->x -= vec.x;
    this->y -= vec.y;
    return *this;
}

Vector2D& Vector2D::multiply(const Vector2D& vec) {
    this->x *= vec.x;
    this->y *= vec.y;
    return *this;
}

Vector2D& Vector2D::divide(const Vector2D& vec) {
    this->x /= vec.x;
    this->y /= vec.y;
    return *this;
}

float Vector2D::dotProduct(const Vector2D& vec) const {
    return x*vec.x + y*vec.y;
}

float Vector2D::dotProduct(const Vector2D& v1, const Vector2D& v2) {
    return v1.dotProduct(v2);
}

float Vector2D::crossProduct(const Vector2D& vec) const {
    return x*vec.y - y*vec.x;
}

float Vector2D::crossProduct(const Vector2D& v1, const Vector2D& v2) {
    return v1.x*v2.y - v1.y*v2.x;
}

float Vector2D::norm2() const {
    return x*x + y*y;
}

float Vector2D::norm() const {
    return sqrt(norm2());
}

void Vector2D::normalize() {
    float a(norm());
    if (a != 0.0f) *this /= a;
}

void Vector2D::zero() {
    x = y = 0;
}

void Vector2D::set(float x_, float y_) {
    x = x_;
    y = y_;
}

bool Vector2D::nonzero() {
    if (x != 0 or y != 0)
        return true;
    return false;
}


Vector2D& Vector2D::operator+=(Vector2D& vec) {
    return this->add(vec);
}

Vector2D& Vector2D::operator-=(Vector2D& vec) {
    return this->subtract(vec);
}

Vector2D& Vector2D::operator*=(Vector2D& vec) {
    return this->multiply(vec);
}

Vector2D& Vector2D::operator/=(Vector2D& vec) {
    return this->divide(vec);
}

Vector2D& Vector2D::operator+=(float a) {
    x += a; y += a;
    return *this;
}

Vector2D& Vector2D::operator-=(float a) {
    x -= a; y -= a;
    return *this;
}

Vector2D& Vector2D::operator*=(float a) {
    x *= a; y *= a;
    return *this;
}

Vector2D& Vector2D::operator/=(float a) {
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
    return v->multiply(v2);
}

Vector2D& operator/(const Vector2D& v1, const Vector2D& v2) {
    Vector2D* v = new Vector2D(v1);
    return v->divide(v2);
}

Vector2D& operator+(const Vector2D& v1, float a) {
    Vector2D* v = new Vector2D(v1);
    return *v += a;
}
Vector2D& operator-(const Vector2D& v1, float a) {
    Vector2D* v = new Vector2D(v1);
    return *v -= a;
}
Vector2D& operator*(float a, const Vector2D& vec) {
    Vector2D* v = new Vector2D(vec);
    return *v *= a;
}
Vector2D& operator*(const Vector2D& v1, float a) {
    Vector2D* v = new Vector2D(v1);
    return *v *= a;
}
Vector2D& operator/(const Vector2D& v1, float a) {
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
