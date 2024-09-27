#include <iostream>
#include <limits>
#include <cmath>
#include "Geometry.hpp"
#include "Constants.hpp"

// PRL_GeometricObject
PRL_GeometricObject::PRL_GeometricObject() {
    aabb_ = {0, 0, 0, 0};
}

PRL_GeometricObject::~PRL_GeometricObject() {}

const PRL_Rect& PRL_GeometricObject::getAABB() const {
    return this->aabb_;
}


// PRL_Line
PRL_Line::PRL_Line(const Vector2D& p1, const Vector2D& p2) : p1_(p1), p2_(p2) {}

PRL_Line::PRL_Line(float x1, float y1, float x2, float y2) : p1_(x1, y1), p2_(x2, y2) {}

PRL_Line::PRL_Line() : p1_(0, 0), p2_(0, 0) {}

PRL_Line::~PRL_Line() {}

void PRL_Line::set(const Vector2D& p1, const Vector2D& p2) {
    this->p1_ = p1;
    this->p2_ = p2;
}

void PRL_Line::set(float x1, float y1, float x2, float y2) {
    this->p1_.set(x1, y1);
    this->p2_.set(x2, y2);
}

bool PRL_Line::isOrdered() const {
    return p1_.x <= p2_.x;
}

void PRL_Line::order() {
    if (p1_.x > p2_.x) {
        float temp(p1_.x);
        p1_.x = p2_.x;
        p2_.x = temp;
        temp = p1_.y;
        p1_.y = p2_.y;
        p2_.y = temp;
    }
}

float PRL_Line::length() const {
    return (p2_ - p1_).norm();
}

float PRL_Line::length2() const {
    return (p2_ - p1_).norm2();
}

float PRL_Line::slope() const {
    if (p2_.y == p1_.y) return 0;
    else if (p2_.x == p1_.x) return std::numeric_limits<float>::infinity();
    else return (p2_.y - p1_.y) / (p2_.x - p1_.x);
}

float PRL_Line::angle() const {
    return atan2(p2_.y - p1_.y, p2_.x - p1_.x);
}

float PRL_Line::area() const {
    return 0;
}

float PRL_Line::perimeter() const {
    return length();
}

void PRL_Line::computeAABB_() {
    aabb_.x = std::min(p1_.x, p2_.x);
    aabb_.y = std::min(p1_.y, p2_.y);
    aabb_.w = std::abs(p2_.x - p1_.x);
    aabb_.h = std::abs(p2_.y - p1_.y);
}

const Vector2D& PRL_Line::p1() const {
    return p1_;
}

const Vector2D& PRL_Line::p2() const {
    return p2_;
}


// PRL_Circle
PRL_Circle::PRL_Circle(float x, float y, float r) : x_(x), y_(y), r_(r) {
    computeAABB_();
}

PRL_Circle::PRL_Circle() : x_(0), y_(0), r_(0) {}

PRL_Circle::~PRL_Circle() {}

void PRL_Circle::set(float x, float y) {
    this->x_ = x;
    this->y_ = y;
    computeAABB_();
}

void PRL_Circle::set(float x, float y, float r) {
    this->x_ = x;
    this->y_ = y;
    this->r_ = r;
    computeAABB_();
}

void PRL_Circle::set(float r) {
    this->r_ = r;
    computeAABB_();
}

float PRL_Circle::x() const {
    return x_;
}

float PRL_Circle::y() const {
    return y_;
}

float PRL_Circle::r() const {
    return r_;
}

float PRL_Circle::area() const {
    return M_PI_F * r_ * r_;
}

float PRL_Circle::perimeter() const {
    return 2 * M_PI_F * r_;
}

void PRL_Circle::computeAABB_() {
    this->aabb_.x = x_ - r_;
    this->aabb_.y = y_ - r_;
    this->aabb_.w = r_ * 2;
    this->aabb_.h = this->aabb_.w;
}

// PRL_Polygon
PRL_Polygon::PRL_Polygon(const std::vector<Vector2D>& vertices) : vertices_(vertices) {
    computeAABB_();
    computeConvex_();
}

PRL_Polygon::PRL_Polygon() {
    vertices_ = { Vector2D(0, 0), Vector2D(0, 1), Vector2D(1, 0)};
    computeAABB_();
    computeConvex_();
}

PRL_Polygon::~PRL_Polygon() {}

void PRL_Polygon::set(const std::vector<Vector2D>& vertices) {
    vertices_ = vertices;
    computeAABB_();
    computeConvex_();
}

const std::vector<Vector2D>& PRL_Polygon::get() const {
    return vertices_;
}

const Vector2D& PRL_Polygon::get(size_t n) const {
    return vertices_[n];
}

size_t PRL_Polygon::size() const {
    return vertices_.size();
}

float PRL_Polygon::area() const {
    float area(0);
    size_t n = vertices_.size();
    for (size_t i = 0; i < n; ++i) {
        area += vertices_[i].x * vertices_[(i + 1) % n].y - vertices_[i].y * vertices_[(i + 1) % n].x;
    }
    return area / 2;
}

float PRL_Polygon::perimeter() const {
    float perimeter(0);
    size_t n = vertices_.size();
    for (size_t i = 0; i < n; ++i) {
        perimeter += (vertices_[i] - vertices_[(i + 1) % n]).norm();
    }
    return perimeter;
}

bool PRL_Polygon::isConvex() const {
    return isConvex_;
}

bool PRL_Polygon::areVerticesPos() const {
    for (size_t i = 0; i < vertices_.size(); ++i) {
        if (vertices_[i].x < 0 || vertices_[i].y < 0) return false;
    }
    return true;
}

bool PRL_Polygon::areVerticesMinPos() const {
    float min_x(std::numeric_limits<float>::max()), min_y(std::numeric_limits<float>::max());
    float max_x(std::numeric_limits<float>::min()), max_y(std::numeric_limits<float>::min());

    for (size_t i = 0; i < vertices_.size(); ++i) {
        if (vertices_[i].x < min_x) min_x = vertices_[i].x;
        if (vertices_[i].y < min_y) min_y = vertices_[i].y;
        if (vertices_[i].x > max_x) max_x = vertices_[i].x;
        if (vertices_[i].y > max_y) max_y = vertices_[i].y;
    }

    if (min_x != 0.0f || min_y != 0.0f) return false;
    else return true;
}

void PRL_Polygon::makeVerticesMinPos() {
    if (areVerticesMinPos()) return;

    float min_x(std::numeric_limits<float>::max()), min_y(std::numeric_limits<float>::max());
    float max_x(std::numeric_limits<float>::min()), max_y(std::numeric_limits<float>::min());

    for (size_t i = 0; i < vertices_.size(); ++i) {
        if (vertices_[i].x < min_x) min_x = vertices_[i].x;
        if (vertices_[i].y < min_y) min_y = vertices_[i].y;
        if (vertices_[i].x > max_x) max_x = vertices_[i].x;
        if (vertices_[i].y > max_y) max_y = vertices_[i].y;
    }

    for (size_t i = 0; i < vertices_.size(); ++i) {
        vertices_[i].x -= min_x;
        vertices_[i].y -= min_y;
    }
    // Update rect_around_ as well, without needing to recompute all of it
    this->aabb_.x -= min_x;
    this->aabb_.y -= min_y;
}

void PRL_Polygon::computeAABB_() {
    float min_x = vertices_[0].x;
    float min_y = vertices_[0].y;
    float max_x = vertices_[0].x;
    float max_y = vertices_[0].y;
    for (size_t i = 1; i < vertices_.size(); ++i) {
        if (vertices_[i].x < min_x) min_x = vertices_[i].x;
        if (vertices_[i].y < min_y) min_y = vertices_[i].y;
        if (vertices_[i].x > max_x) max_x = vertices_[i].x;
        if (vertices_[i].y > max_y) max_y = vertices_[i].y;
    }
    this->aabb_.x = min_x;
    this->aabb_.y = min_y;
    this->aabb_.w = max_x - min_x;
    this->aabb_.h = max_y - min_y;
}

void PRL_Polygon::computeConvex_() {
    size_t n = vertices_.size();
    if (n < 3) {
        isConvex_ = false;
        return;
    }

    bool isConvex = true;
    bool sign = false;
    for (size_t i = 0; i < n; ++i) {
        float dx1 = vertices_[(i + 2) % n].x - vertices_[(i + 1) % n].x;
        float dy1 = vertices_[(i + 2) % n].y - vertices_[(i + 1) % n].y;
        float dx2 = vertices_[i].x - vertices_[(i + 1) % n].x;
        float dy2 = vertices_[i].y - vertices_[(i + 1) % n].y;
        float cross = dx1 * dy2 - dy1 * dx2;
        if (i == 0) sign = cross > 0;
        else if (sign != (cross > 0)) {
            isConvex = false;
            break;
        }
    }
    isConvex_ = isConvex;
}

// PolygonGenerator
PolygonGenerator::~PolygonGenerator() {}

PRL_Polygon PolygonGenerator::generate(size_t n, float r, bool coordFromCenter) {
    return generate(n, r, 0, coordFromCenter);
}

PRL_Polygon PolygonGenerator::generate(size_t n, float r, float rotAngle, bool coordFromCenter) {
    if (n < 3) {
        std::cerr << "Polygon must have at least 3 vertices. Exiting..." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if (r <= 0) {
        std::cerr << "Radius must be strictly positive. Exiting..." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::vector<Vector2D> vertices;
    double angle = M_2PI_D / n;
    if (coordFromCenter){
        for (size_t i = 0; i < n; ++i) {
            vertices.push_back(Vector2D(r * cos(static_cast<float>(angle * i) + rotAngle), 
                                        r * sin(static_cast<float>(angle * i) + rotAngle)));
        }
    }
    else {
        for (size_t i = 0; i < n; ++i) {
            vertices.push_back(Vector2D(r * cos(static_cast<float>(angle * i) + rotAngle) + r, 
                                        r * sin(static_cast<float>(angle * i) + rotAngle) + r));
        }
    }
    return PRL_Polygon(vertices);
}

// Stream operators
std::ostream& operator<<(std::ostream& os, const PRL_Line& line) {
    os << "P1: " << line.p1_ << std::endl;
    os << "P2: " << line.p2_ << std::endl;
    os << "Length: " << line.length() << std::endl;
    os << "Slope: " << line.slope() << std::endl;
    os << "Angle: " << line.angle() << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const PRL_Circle& cir) {
    os << "Center: (" << cir.x_ << ", " << cir.y_ << ")" << std::endl;
    os << "Radius: " << cir.r_ << std::endl;
    os << "Area: " << cir.area() << std::endl;
    os << "Perimeter: " << cir.perimeter() << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const PRL_Polygon& poly) {
    os << "Vertices (" << poly.vertices_.size() << "): " << std::endl;
    for (size_t i = 0; i < poly.vertices_.size(); ++i) {
        os << i << ". " << poly.vertices_[i] << std::endl;
    }
    os << "Area: " << poly.area() << std::endl;
    os << "Perimeter: " << poly.perimeter() << std::endl;
    return os;
}
