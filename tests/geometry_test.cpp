#include "Geometry.hpp"
#include "Constants.hpp"
#include <gtest/gtest.h>
#include <cmath>

constexpr float minTol(2e-15);

// PRL_Line tests
TEST(PRL_Line, DefaultConstructor) {
    PRL_Line line;
    EXPECT_EQ(line.p1().x, 0);
    EXPECT_EQ(line.p1().y, 0);
    EXPECT_EQ(line.p2().x, 0);
    EXPECT_EQ(line.p2().y, 0);
}

TEST(PRL_Line, CopyConstructor) {
    PRL_Line line(Vector2D(-1, 2), Vector2D(3, 4));
    PRL_Line line2(line);
    EXPECT_EQ(line2.p1().x, line.p1().x);
    EXPECT_EQ(line2.p1().y, line.p1().y);
    EXPECT_EQ(line2.p2().x, line.p2().x);
    EXPECT_EQ(line2.p2().y, line.p2().y);
}

TEST(PRL_Line, AssignmentOperator) {
    PRL_Line line(Vector2D(-1, 2), Vector2D(3, 4));
    PRL_Line line2;
    line2 = line;
    EXPECT_EQ(line2.p1().x, line.p1().x);
    EXPECT_EQ(line2.p1().y, line.p1().y);
    EXPECT_EQ(line2.p2().x, line.p2().x);
    EXPECT_EQ(line2.p2().y, line.p2().y);
}

TEST(PRL_Line, Constructor) {
    PRL_Line line(Vector2D(-1, 2), Vector2D(3, 4));
    EXPECT_EQ(line.p1().x, -1);
    EXPECT_EQ(line.p1().y, 2);
    EXPECT_EQ(line.p2().x, 3);
    EXPECT_EQ(line.p2().y, 4);
}

TEST(PRL_Line, SettersAndGetters) {
    PRL_Line line(Vector2D(-1, 2), Vector2D(3, 4));
    line.set(Vector2D(5, 6), Vector2D(7, 8));
    EXPECT_EQ(line.p1().x, 5);
    EXPECT_EQ(line.p1().y, 6);
    EXPECT_EQ(line.p2().x, 7);
    EXPECT_EQ(line.p2().y, 8);

    line.set(9, 10, 11, 12);
    EXPECT_EQ(line.p1().x, 9);
    EXPECT_EQ(line.p1().y, 10);
    EXPECT_EQ(line.p2().x, 11);
    EXPECT_EQ(line.p2().y, 12);
}

TEST(PRL_Line, IsOrdered) {
    PRL_Line line(Vector2D(3, 4), Vector2D(-1, 2));
    EXPECT_FALSE(line.isOrdered());
}

TEST(PRL_Line, Order) {
    PRL_Line line(Vector2D(3, 4), Vector2D(-1, 2));
    line.order();
    EXPECT_TRUE(line.isOrdered());
}

TEST(PRL_Line, Length) {
    PRL_Line line(Vector2D(-1, 2), Vector2D(3, 4));
    EXPECT_EQ(line.length(), sqrt((3.0f + 1.0f) * (3.0f + 1.0f) + (4.0f - 2.0f) * (4.0f - 2.0f)));
}

TEST(PRL_Line, Length2) {
    PRL_Line line(Vector2D(-1, 2), Vector2D(3, 4));
    EXPECT_EQ(line.length2(), (3 + 1) * (3 + 1) + (4 - 2) * (4 - 2));
}

TEST(PRL_Line, Slope) {
    PRL_Line line(Vector2D(-1, 2), Vector2D(3, 4));
    EXPECT_EQ(line.slope(), (4.0f - 2.0f) / (3.0f + 1.0f));
}

TEST(PRL_Line, Angle) {
    PRL_Line line(Vector2D(-1, 2), Vector2D(3, 4));
    EXPECT_EQ(line.angle(), atan2(4.0f - 2.0f, 3.0f + 1.0f));
}


// PRL_Circle tests
TEST(PRL_Circle, DefaultConstructor) {
    PRL_Circle circle;
    EXPECT_EQ(circle.x(), 0);
    EXPECT_EQ(circle.y(), 0);
    EXPECT_EQ(circle.r(), 0);
}

TEST(PRL_Circle, CopyConstructor) {
    PRL_Circle circle(-1, 2, 5);
    PRL_Circle circle2(circle);
    EXPECT_EQ(circle2.x(), circle.x());
    EXPECT_EQ(circle2.y(), circle.y());
    EXPECT_EQ(circle2.r(), circle.r());
}

TEST(PRL_Circle, AssignmentOperator) {
    PRL_Circle circle(-1, 2, 5);
    PRL_Circle circle2;
    circle2 = circle;
    EXPECT_EQ(circle2.x(), circle.x());
    EXPECT_EQ(circle2.y(), circle.y());
    EXPECT_EQ(circle2.r(), circle.r());
}

TEST(PRL_Circle, Constructor) {
    PRL_Circle circle(-1, 2, 5);
    EXPECT_EQ(circle.x(), -1);
    EXPECT_EQ(circle.y(), 2);
    EXPECT_EQ(circle.r(), 5);
}

TEST(PRL_Circle, SettersAndGetters) {
    PRL_Circle circle(0, 0, 5);
    circle.set(10, 20, 30);
    EXPECT_EQ(circle.x(), 10);
    EXPECT_EQ(circle.y(), 20);
    EXPECT_EQ(circle.r(), 30);

    circle.set(40, 50);
    EXPECT_EQ(circle.x(), 40);
    EXPECT_EQ(circle.y(), 50);

    circle.set(60);
    EXPECT_EQ(circle.r(), 60);
}

TEST(PRL_Circle, Area) {
    PRL_Circle circle(-1, 2, 5);
    EXPECT_NEAR(circle.area(), float(M_PI_F) * circle.r() * circle.r(), minTol);
}

TEST(PRL_Circle, Circumference) {
    PRL_Circle circle(-1, 2, 5);
    EXPECT_NEAR(circle.perimeter(), 2 * M_PI_F * circle.r(), minTol);
}

TEST(PRL_Circle, RectAround) {
    PRL_Circle circle(-1, 2, 5);
    EXPECT_EQ(circle.getAABB().x, circle.x() - circle.r());
    EXPECT_EQ(circle.getAABB().y, circle.y() - circle.r());
    EXPECT_EQ(circle.getAABB().w, circle.r() * 2);
    EXPECT_EQ(circle.getAABB().h, circle.r() * 2);
}


// PRL_Polygon tests
TEST(PRL_Polygon, DefaultConstructor) {
    PRL_Polygon poly;
    EXPECT_EQ(poly.size(), 3);
}

TEST(PRL_Polygon, CopyConstructor) {
    std::vector<Vector2D> vertices = { Vector2D(-1, 2), Vector2D(3, 4), Vector2D(5, 6) };
    PRL_Polygon poly(vertices);
    PRL_Polygon poly2(poly);
    EXPECT_EQ(poly2.size(), poly.size());
    EXPECT_EQ(poly2.get(0).x, poly.get(0).x);
    EXPECT_EQ(poly2.get(0).y, poly.get(0).y);
    EXPECT_EQ(poly2.get(1).x, poly.get(1).x);
    EXPECT_EQ(poly2.get(1).y, poly.get(1).y);
    EXPECT_EQ(poly2.get(2).x, poly.get(2).x);
    EXPECT_EQ(poly2.get(2).y, poly.get(2).y);
}

TEST(PRL_Polygon, AssignmentOperator) {
    std::vector<Vector2D> vertices = { Vector2D(-1, 2), Vector2D(3, 4), Vector2D(5, 6) };
    PRL_Polygon poly(vertices);
    PRL_Polygon poly2;
    poly2 = poly;
    EXPECT_EQ(poly2.size(), poly.size());
    EXPECT_EQ(poly2.get(0).x, poly.get(0).x);
    EXPECT_EQ(poly2.get(0).y, poly.get(0).y);
    EXPECT_EQ(poly2.get(1).x, poly.get(1).x);
    EXPECT_EQ(poly2.get(1).y, poly.get(1).y);
    EXPECT_EQ(poly2.get(2).x, poly.get(2).x);
    EXPECT_EQ(poly2.get(2).y, poly.get(2).y);
}

TEST(PRL_Polygon, Constructor) {
    std::vector<Vector2D> vertices = { Vector2D(-1, 2), Vector2D(3, 4), Vector2D(5, 6) };
    PRL_Polygon poly(vertices);
    EXPECT_EQ(poly.size(), 3);
    EXPECT_EQ(poly.get(0).x, -1);
    EXPECT_EQ(poly.get(0).y, 2);
    EXPECT_EQ(poly.get(1).x, 3);
    EXPECT_EQ(poly.get(1).y, 4);
    EXPECT_EQ(poly.get(2).x, 5);
    EXPECT_EQ(poly.get(2).y, 6);
}

TEST(PRL_Polygon, SettersAndGetters) {
    std::vector<Vector2D> vertices = { Vector2D(-1, 2), Vector2D(3, 4), Vector2D(5, 6) };
    PRL_Polygon poly;
    poly.set(vertices);
    EXPECT_EQ(poly.size(), 3);
    EXPECT_EQ(poly.get(0).x, -1);
    EXPECT_EQ(poly.get(0).y, 2);
    EXPECT_EQ(poly.get(1).x, 3);
    EXPECT_EQ(poly.get(1).y, 4);
    EXPECT_EQ(poly.get(2).x, 5);
    EXPECT_EQ(poly.get(2).y, 6);
}

TEST(PRL_Polygon, AABB) {
    std::vector<Vector2D> vertices = { Vector2D(-1, 2), Vector2D(3, 4), Vector2D(5, 6) };
    PRL_Polygon poly(vertices);
    EXPECT_EQ(poly.getAABB().x, -1);
    EXPECT_EQ(poly.getAABB().y, 2);
    EXPECT_EQ(poly.getAABB().w, 6);
    EXPECT_EQ(poly.getAABB().h, 4);
}

TEST(PRL_Polygon, Area) {
    std::vector<Vector2D> vertices = { Vector2D(-1, 2), Vector2D(3, 4), Vector2D(5, 6) };
    PRL_Polygon poly(vertices);
    EXPECT_NEAR(poly.area(), 0.5 * abs((3 - (-1)) * (6 - 2) - (5 - (-1)) * (4 - 2)), minTol);
}

TEST(PRL_Polygon, Perimeter) {
    std::vector<Vector2D> vertices = { Vector2D(-1, 2), Vector2D(3, 4), Vector2D(5, 6) };
    PRL_Polygon poly(vertices);
    // Write this line with floats to avoid implicit conversion to double
    EXPECT_NEAR(poly.perimeter(), sqrt((3.0f - (-1.0f)) * (3.0f - (-1.0f)) + 
        (4.0f - 2.0f) * (4.0f - 2.0f)) +
        sqrt((5.0f - 3.0f) * (5.0f - 3.0f) + (6.0f - 4.0f) * (6.0f - 4.0f)) +
        sqrt((5.0f - (-1.0f)) * (5.0f - (-1.0f)) + (6.0f - 2.0f) * (6.0f - 2.0f)), minTol);
}

TEST(PRL_Polygon, IsConvex) {
    std::vector<Vector2D> vertices = { Vector2D(0, 0), Vector2D(3, 0), Vector2D(1, 1), Vector2D(0, 1) };
    PRL_Polygon poly(vertices);
    EXPECT_TRUE(poly.isConvex());
    vertices = { Vector2D(0, 0), Vector2D(1, 0), Vector2D(0.5, 0.5), Vector2D(1, 1), Vector2D(0, 1) };
    poly.set(vertices);
    EXPECT_FALSE(poly.isConvex());
}

// PolygonGenerator tests
TEST(PolygonGenerator, Pentagon) {
    PRL_Polygon pentagon = PolygonGenerator::generate(5, 1);
    EXPECT_EQ(pentagon.size(), 5);
    EXPECT_EQ(pentagon.get(0).x, 1);
    EXPECT_EQ(pentagon.get(0).y, 0);
    EXPECT_EQ(pentagon.get(1).x, cos(2 * M_PI_F / 5));
    EXPECT_EQ(pentagon.get(1).y, sin(2 * M_PI_F / 5));
    EXPECT_EQ(pentagon.get(2).x, cos(4 * M_PI_F / 5));
    EXPECT_EQ(pentagon.get(2).y, sin(4 * M_PI_F / 5));
    EXPECT_EQ(pentagon.get(3).x, cos(6 * M_PI_F / 5));
    EXPECT_EQ(pentagon.get(3).y, sin(6 * M_PI_F / 5));
    EXPECT_EQ(pentagon.get(4).x, cos(8 * M_PI_F / 5));
    EXPECT_EQ(pentagon.get(4).y, sin(8 * M_PI_F / 5));
}

TEST(PolygonGenerator, Hexagon) { // Specific way of computing sides
    int n = 59;
    float angle = static_cast<float>(M_PI_D / 12);
    PRL_Polygon hexagon = PolygonGenerator::generate(n, 1, angle);
    
    EXPECT_EQ(hexagon.size(), n);
    for (size_t i(0); i < n; ++i) {
        EXPECT_EQ(hexagon.get(i).x, cos((float)(M_2PI_D / n * i) + angle));
        EXPECT_EQ(hexagon.get(i).y, sin((float)(M_2PI_D / n * i) + angle));
    }
}

TEST(PolygonGenerator, N3_to_999_R2_Polygon) {
    float angle = static_cast<float>(M_PI_D / 12);
    float r(2);

    for (int n(3); n < 1000; ++n) {
        PRL_Polygon poly = PolygonGenerator::generate(n, r, angle);
        EXPECT_EQ(poly.size(), n);
        for (size_t i(0); i < n; ++i) {
            EXPECT_EQ(poly.get(i).x, r * cos((float)(M_2PI_D / n * i) + angle));
            EXPECT_EQ(poly.get(i).y, r * sin((float)(M_2PI_D / n * i) + angle));
        }
    }
}