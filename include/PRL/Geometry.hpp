#ifndef GEOMETRY_HPP_INCLUDED
#define GEOMETRY_HPP_INCLUDED

#include <iostream>
#include <vector>
#include "Types.hpp"
#include "Vector2D.hpp"

class Collision;

class PRL_GeometricObject {
public:
    PRL_GeometricObject();
    virtual ~PRL_GeometricObject() = 0;

    virtual float area() const = 0;
    virtual float perimeter() const = 0;
    const PRL_Rect& getAABB() const;

protected:
    PRL_Rect aabb_;
    virtual void computeAABB_() = 0;

    friend class Collision;
};

class PRL_Line : public PRL_GeometricObject {
public:
    PRL_Line(const Vector2D& p1, const Vector2D& p2);
    PRL_Line(float x1, float y1, float x2, float y2);
    PRL_Line();
    ~PRL_Line();

    float area() const override;
    float perimeter() const override;

    void set(const Vector2D& p1, const Vector2D& p2);
    void set(float x1, float y1, float x2, float y2);
    bool isOrdered() const;
    void order();
    float length() const;
    float length2() const;
    float slope() const;
    float angle() const;
    const Vector2D& p1() const;
    const Vector2D& p2() const;

private:
    void computeAABB_() override;
    Vector2D p1_, p2_;

    friend class Collision;
    friend std::ostream& operator<<(std::ostream& os, const PRL_Line& vec);
};

class PRL_Circle : public PRL_GeometricObject {
public:
    PRL_Circle(float x, float y, float r);
    PRL_Circle();
    ~PRL_Circle();

    float area() const override;
    float perimeter() const override;

    void set(float x, float y);
    void set(float x, float y, float r);
    void set(float r);
    float x() const;
    float y() const;
    float r() const;

private:
    void computeAABB_() override;
    float x_, y_, r_;

    friend class Collision;
    friend std::ostream& operator<<(std::ostream& os, const PRL_Circle& cir);
};


class PRL_Polygon : public PRL_GeometricObject {
public:
    PRL_Polygon(const std::vector<Vector2D>& vertices);
    PRL_Polygon();
    ~PRL_Polygon();

    float area() const override;
    float perimeter() const override;

    void set(const std::vector<Vector2D>& vertices);
    const std::vector<Vector2D>& get() const;
    const Vector2D& get(size_t n) const;
    const PRL_Rect& getRect() const;
    size_t size() const;
    bool isConvex() const;
    bool areVerticesPos() const;
    bool areVerticesMinPos() const;
    void makeVerticesMinPos();

private:
    void computeAABB_() override;
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
std::ostream& operator<<(std::ostream& os, const PRL_Circle& cir);
std::ostream& operator<<(std::ostream& os, const PRL_Polygon& poly);

#endif // GEOMETRY_HPP_INCLUDED