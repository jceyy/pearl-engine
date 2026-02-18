#ifndef VECTOR_2D_HPP
#define VECTOR_2D_HPP

#include <iostream>
#include <cmath>
#include <cassert>
#include "Types.hpp"


//! \brief A simple 2D vector class with basic operations and operator overloads.
template<typename T>
class Vec2D {
    static_assert(std::is_arithmetic<T>::value, "Vec2D requires arithmetic type");
public:
    Vec2D() : x(0), y(0) {}
    Vec2D(T x, T y) : x(x), y(y) {}

    template<typename U>
    using norm_t = typename std::conditional<std::is_floating_point<U>::value, U, float>::type;
    norm_t<T> inline norm() const {
        return static_cast<norm_t<T>>(sqrt(x*x + y*y));
    }
    inline T norm2() const {
        return x*x + y*y;
    }

    template<typename U = T>
    typename std::enable_if<std::is_floating_point<U>::value, void>::type
    normalize() {
        auto n = norm();
        if (n != 0) {
            x /= n;
            y /= n;
        }
    }
    inline void zero() { x = y = static_cast<T>(0); }
    inline void set(T x, T y) { this->x = x; this->y = y; }
    inline bool nonzero() const { 
        return x != static_cast<T>(0) || y != static_cast<T>(0); 
    }

    // Dot / cross
    template<typename U>
    constexpr std::common_type_t<T, U> dot(const Vec2D<U>& v) const {
        using R = std::common_type_t<T, U>;
        return static_cast<R>(x)*static_cast<R>(v.x) + static_cast<R>(y)*static_cast<R>(v.y);
    }

    template<typename U>
    constexpr std::common_type_t<T, U> cross(const Vec2D<U>& v) const {
        using R = std::common_type_t<T, U>;
        return static_cast<R>(x)*static_cast<R>(v.y) - static_cast<R>(y)*static_cast<R>(v.x);
    }

    // Componentwise ops
    template<typename U> inline Vec2D<T>& operator+=(const Vec2D<U>& vec) {
        x += static_cast<T>(vec.x);
        y += static_cast<T>(vec.y);
        return *this;
    }

    template<typename U> inline Vec2D<T>& operator-=(const Vec2D<U>& vec) {
        x -= static_cast<T>(vec.x);
        y -= static_cast<T>(vec.y);
        return *this;
    }

    template<typename U> inline Vec2D<T>& operator*=(const Vec2D<U>& vec) {
        this->x = static_cast<T>(this->x * vec.x);
        this->y = static_cast<T>(this->y * vec.y);
        return *this;
    }

    template<typename U> inline Vec2D<T>& operator/=(const Vec2D<U>& vec) {
        assert(vec.x != static_cast<U>(0) && vec.y != static_cast<U>(0) && "Division by zero in Vec2D::divideComponentwise");
        this->x = static_cast<T>(this->x / vec.x);
        this->y = static_cast<T>(this->y / vec.y);
        return *this;
    }

    template<typename U> inline Vec2D<T>& operator*=(U s) {
        static_assert(std::is_arithmetic<U>::value, "Vec2D requires arithmetic type");
        x *= static_cast<T>(s);
        y *= static_cast<T>(s);
        return *this;
    }

    template<typename U> inline Vec2D<T>& operator/=(U s) {
        static_assert(std::is_arithmetic<U>::value, "Vec2D requires arithmetic type");
        assert(s != 0);
        x /= static_cast<T>(s);
        y /= static_cast<T>(s);
        return *this;
    }

    template<typename U> inline Vec2D<T>& operator+=(U s) {
        static_assert(std::is_arithmetic<U>::value, "Vec2D requires arithmetic type");
        assert(s != 0);
        x += static_cast<T>(s);
        y += static_cast<T>(s);
        return *this;
    }

    template<typename U> inline Vec2D<T>& operator-=(U s) {
        static_assert(std::is_arithmetic<U>::value, "Vec2D requires arithmetic type");
        assert(s != 0);
        x -= static_cast<T>(s);
        y -= static_cast<T>(s);
        return *this;
    }

    T x;
    T y;
};

// Vec2D operators
template<typename T, typename U>
constexpr Vec2D<T> operator+(Vec2D<T> a, const Vec2D<U>& b) {
    static_assert(std::is_arithmetic<U>::value, "Vec2D requires arithmetic type");
    return a += b;
}

template<typename T, typename U>
constexpr Vec2D<T> operator-(Vec2D<T> a, const Vec2D<U>& b) {
    static_assert(std::is_arithmetic<U>::value, "Vec2D requires arithmetic type");
    return a -= b;
}

template<typename T, typename U>
constexpr Vec2D<T> operator*(Vec2D<T> a, const Vec2D<U>& b) {
    static_assert(std::is_arithmetic<U>::value, "Vec2D requires arithmetic type");
    return a *= b;
}

template<typename T, typename U>
constexpr Vec2D<T> operator/(Vec2D<T> a, const Vec2D<U>& b) {
    static_assert(std::is_arithmetic<U>::value, "Vec2D requires arithmetic type");
    return a /= b;
}

// Scalar ops
template<typename T, typename U>
constexpr Vec2D<T> operator*(Vec2D<T> v, U s) {
    static_assert(std::is_arithmetic<U>::value, "Vec2D requires arithmetic type");
    return v *= s;
}

template<typename T, typename U>
constexpr Vec2D<T> operator*(U s, Vec2D<T> v) {
    static_assert(std::is_arithmetic<U>::value, "Vec2D requires arithmetic type");
    return v *= s;
}

template<typename T, typename U>
constexpr Vec2D<T> operator/(Vec2D<T> v, U s) {
    static_assert(std::is_arithmetic<U>::value, "Vec2D requires arithmetic type");
    return v /= s;
}

template<typename T, typename U>
constexpr Vec2D<T> operator/(U s, Vec2D<T> v) {
    static_assert(std::is_arithmetic<U>::value, "Vec2D requires arithmetic type");
    return v /= s;
}

template<typename T, typename U>
constexpr Vec2D<T> operator+(Vec2D<T> v, U s) {
    static_assert(std::is_arithmetic<U>::value, "Vec2D requires arithmetic type");
    return v += s;
}

template<typename T, typename U>
constexpr Vec2D<T> operator+(U s, Vec2D<T> v) {
    static_assert(std::is_arithmetic<U>::value, "Vec2D requires arithmetic type");
    return v += s;
}

template<typename T, typename U>
constexpr Vec2D<T> operator-(Vec2D<T> v, U s) {
    static_assert(std::is_arithmetic<U>::value, "Vec2D requires arithmetic type");
    return v -= s;
}

template<typename T, typename U>
constexpr Vec2D<T> operator-(U s, Vec2D<T> v) {
    static_assert(std::is_arithmetic<U>::value, "Vec2D requires arithmetic type");
    return v -= s;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vec2D<T>& v) {
    return os << "(" << v.x << ", " << v.y << ")";
}

//! Convenience typedef for 2D vector with PosType components
using Vector2D = Vec2D<PosType>;

#endif // VECTOR_2D_HPP