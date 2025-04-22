#ifndef GEOMETRY_HPP_INCLUDED
#define GEOMETRY_HPP_INCLUDED

#include <iostream>
#include <vector>
#include "Types.hpp"
#include "Vector2D.hpp"

class Collision;

class PRL_GeometricObject {
public:
    PRL_GeometricObject() = default;
    virtual ~PRL_GeometricObject() = 0;

    virtual float area() const noexcept = 0;
    virtual float perimeter() const noexcept = 0;

protected:
    friend class Collision;
};

class PRL_Line : public PRL_GeometricObject {
public:
    PRL_Line() = delete;
    PRL_Line(const PRL_Line& line) = default;
    PRL_Line(const Vector2D& p1, const Vector2D& p2);
    PRL_Line(float x1, float y1, float x2, float y2);
    ~PRL_Line() = default;

    float area() const noexcept override;
    float perimeter() const noexcept override;

    void set(const Vector2D& p1, const Vector2D& p2);
    void set(float x1, float y1, float x2, float y2);
    bool isOrdered() const noexcept;
    void order();
    float length() const noexcept;
    float length2() const noexcept;
    float slope() const noexcept;
    float angle() const;
    inline const Vector2D& p1() const noexcept { return p1_; }
    inline const Vector2D& p2() const noexcept { return p2_; }

private:
    Vector2D p1_, p2_;

    friend class Collision;
    friend std::ostream& operator<<(std::ostream& os, const PRL_Line& vec);
};

class PRL_Rectangle : public PRL_GeometricObject {
public:
    PRL_Rectangle() = delete;
    PRL_Rectangle(const PRL_Rectangle& rec) = default;
    PRL_Rectangle(float x, float y, float w, float h);
    
    ~PRL_Rectangle() = default;

    float area() const noexcept override;
    float perimeter() const noexcept override;

    void set(float x, float y, float w, float h);
    inline float x() const noexcept { return x_; }
    inline float y() const noexcept { return y_; }
    inline float w() const noexcept { return w_; }
    inline float h() const noexcept { return h_; }

private:
    float x_, y_, w_, h_;

    friend class Collision;
    friend std::ostream& operator<<(std::ostream& os, const PRL_Rectangle& rec);
};

class PRL_Circle : public PRL_GeometricObject {
public:
    PRL_Circle() = delete;
    PRL_Circle(const PRL_Circle& cir) = default;
    PRL_Circle(float x, float y, float r);
    
    ~PRL_Circle() = default;

    float area() const noexcept override;
    float perimeter() const noexcept override;

    void set(float x, float y) noexcept;
    void set(float x, float y, float r);
    void set(float r);
    inline float x() const noexcept { return x_; }
    inline float y() const noexcept { return y_; }
    inline float r() const noexcept { return r_; }

private:
    float x_, y_, r_;

    friend class Collision;
    friend std::ostream& operator<<(std::ostream& os, const PRL_Circle& cir);
};


class PRL_Polygon : public PRL_GeometricObject {
public:
    PRL_Polygon() = delete;
    PRL_Polygon(const PRL_Polygon& poly) = default;
    PRL_Polygon(const std::vector<Vector2D>& vertices);
    
    ~PRL_Polygon() = default;

    float area() const noexcept override;
    float perimeter() const noexcept override;

    void set(const std::vector<Vector2D>& vertices);
    const std::vector<Vector2D>& get() const;
    const Vector2D& get(size_t n) const;
    size_t nVertices() const noexcept;
    bool isConvex() const;
    // bool areVerticesPos() const;
    // bool areVerticesMinPos() const;
    // void makeVerticesMinPos();

private:
    void computeConvex_();

    std::vector<Vector2D> vertices_;
    bool isConvex_;

    friend class Collision;
    friend std::ostream& operator<<(std::ostream& os, const PRL_Polygon& poly);
};

// Static class that allows to generate regular polynomials on a circle
class PolygonGenerator {
public:
    virtual ~PolygonGenerator() = 0;
    static PRL_Polygon generate(size_t n, float r, bool coordFromCenter = true);
    static PRL_Polygon generate(size_t n, float r, float rotAngle, bool coordFromCenter = true);
};

std::ostream& operator<<(std::ostream& os, const PRL_Line& vec);
std::ostream& operator<<(std::ostream& os, const PRL_Rectangle& rec);
std::ostream& operator<<(std::ostream& os, const PRL_Circle& cir);
std::ostream& operator<<(std::ostream& os, const PRL_Polygon& poly);

#endif // GEOMETRY_HPP_INCLUDED