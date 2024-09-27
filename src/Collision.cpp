#include "Collision.hpp"
#include "ECS/ColliderComponent.hpp"
#include "Types.hpp"

// PRL_Rect Collision::rect;

bool Collision::ValueInRange(float value, float min, float max) {
    if (value >= min && value <= max) {
        return true;
    }
    else return false;
}

bool Collision::ValueInRangeStrict(float value, float min, float max) {
    if (value > min && value < max) {
        return true;
    }
    else return false;
}

bool Collision::PointLine(const Vector2D& point, const PRL_Line& line) {
    float a = line.p2_.y - line.p1_.y;
    float b = line.p1_.x - line.p2_.x;
    float c = line.p2_.x * line.p1_.y - line.p1_.x * line.p2_.y;
    if (a * point.x + b * point.y + c == 0) {
        return true;
    }
    else return false;
}

bool Collision::PointAABB(const Vector2D& point, const PRL_Rect& rec) {
    if (point.x >= rec.x && point.x <= rec.x + rec.w &&
        point.y >= rec.y && point.y <= rec.y + rec.h) {
        return true;
    }
    else return false;
}

bool Collision::PointCircle(const Vector2D& point, const PRL_Circle& cir) {
    float deltaX = point.x - cir.x_;
    float deltaY = point.y - cir.y_;
    if ((deltaX * deltaX + deltaY * deltaY) < (cir.r_ * cir.r_)) {
        return true;
    }
    else return false;
}

bool Collision::PointPolygon(const Vector2D& point, const PRL_Polygon& poly) {
    std::vector<Vector2D> vertices = poly.vertices_;
    int i, j, nvert = vertices.size();
    bool c = false;

    for (i = 0, j = nvert - 1; i < nvert; j = i++) {
        if (((vertices[i].y > point.y) != (vertices[j].y > point.y)) &&
            (point.x < (vertices[j].x - vertices[i].x) * (point.y - vertices[i].y) / (vertices[j].y - vertices[i].y) + vertices[i].x)) {
            c = !c;
        }
    }
    return c;
}

bool Collision::Line(const PRL_Line& lineA, const PRL_Line& lineB) {
    float a1 = lineA.p2_.y - lineA.p1_.y;
    float b1 = lineA.p1_.x - lineA.p2_.x;
    float c1 = lineA.p2_.x * lineA.p1_.y - lineA.p1_.x * lineA.p2_.y;
    float a2 = lineB.p2_.y - lineB.p1_.y;
    float b2 = lineB.p1_.x - lineB.p2_.x;
    float c2 = lineB.p2_.x * lineB.p1_.y - lineB.p1_.x * lineB.p2_.y;
    float det = a1 * b2 - a2 * b1;
    if (det == 0) {
        return false;
    }
    else {
        float x = (b1 * c2 - b2 * c1) / det;
        float y = (a2 * c1 - a1 * c2) / det;
        if (x >= std::min(lineA.p1_.x, lineA.p2_.x) && x <= std::max(lineA.p1_.x, lineA.p2_.x) &&
            y >= std::min(lineA.p1_.y, lineA.p2_.y) && y <= std::max(lineA.p1_.y, lineA.p2_.y) &&
            x >= std::min(lineB.p1_.x, lineB.p2_.x) && x <= std::max(lineB.p1_.x, lineB.p2_.x) &&
            y >= std::min(lineB.p1_.y, lineB.p2_.y) && y <= std::max(lineB.p1_.y, lineB.p2_.y)) {
            return true;
        }
        else return false;
    }
}

bool Collision::LineAABB(const PRL_Line& line, const PRL_Rect& rec) {
    PRL_Line lineA = { {rec.x, rec.y}, {rec.x + rec.w, rec.y} };
    PRL_Line lineB = { {rec.x + rec.w, rec.y}, {rec.x + rec.w, rec.y + rec.h} };
    PRL_Line lineC = { {rec.x + rec.w, rec.y + rec.h}, {rec.x, rec.y + rec.h} };
    PRL_Line lineD = { {rec.x, rec.y + rec.h}, {rec.x, rec.y} };
    if (Line(line, lineA) || Line(line, lineB) ||
        Line(line, lineC) || Line(line, lineD)) {
        return true;
    }
    else if (PointAABB(line.p1_, rec)) // line inside the rectangle case, testing one point is enough
    {
        return true;
    }
    else return false;
}

bool Collision::AABB(const PRL_Rect& A, const PRL_Rect& B) {
    if (A.x + A.w >= B.x && B.x + B.w >= A.x &&
        A.y + A.h >= B.y && B.y + B.h >= A.y) {
        return true;
    }
    else return false;
}

bool Collision::AABBCircle(const PRL_Rect& rec, const PRL_Circle& cir) {
    // Vertical part
    if (ValueInRange(cir.y_ - cir.r_, rec.y, rec.y + rec.h) ||
        ValueInRange(cir.y_ + cir.r_, rec.y, rec.y + rec.h) ||
        ValueInRange(rec.y, cir.y_ - cir.r_, cir.y_ + cir.r_) ||
        ValueInRange(rec.y + rec.h, cir.y_ - cir.r_, cir.y_ + cir.r_)) {
        return true;
    }
    // Horizontal part
    if (ValueInRange(cir.x_ - cir.r_, rec.x, rec.x + rec.w) ||
        ValueInRange(cir.x_ + cir.r_, rec.x, rec.x + rec.w) ||
        ValueInRange(rec.x, cir.x_ - cir.r_, cir.x_ + cir.r_) ||
        ValueInRange(rec.x + rec.w, cir.x_ - cir.r_, cir.x_ + cir.r_)) {
        return true;
    }
    else return false;
}

bool Collision::Circle(const PRL_Circle& cirA, const PRL_Circle& cirB) {
    float deltaX = cirB.x_ - cirA.x_;
    float deltaY = cirB.y_ - cirA.y_;
    if ((deltaX * deltaX + deltaY * deltaY) < ((cirA.r_ + cirB.r_) * (cirA.r_ + cirB.r_))) {
        return true;
    }
    else return false;
}


bool Collision::Polygon(const PRL_Polygon& polyA, const PRL_Polygon& polyB){
    std::vector<Vector2D> axes;
    std::vector<Vector2D> verticesA = polyA.vertices_;
    std::vector<Vector2D> verticesB = polyB.vertices_;

    // Add the edges of polygon A as axes
    for (size_t i = 0; i < verticesA.size(); i++) {
        Vector2D p1 = verticesA[i];
        Vector2D p2 = verticesA[(i + 1) % verticesA.size()];
        Vector2D edge = p2 - p1;
        Vector2D normal = {edge.y, -edge.x};
        normal.normalize();
        axes.push_back(normal);
    }

    // Add the edges of polygon B as axes
    for (size_t i = 0; i < verticesB.size(); i++) {
        Vector2D p1 = verticesB[i];
        Vector2D p2 = verticesB[(i + 1) % verticesB.size()];
        Vector2D edge = p2 - p1;
        Vector2D normal = {edge.y, -edge.x};
        normal.normalize();
        axes.push_back(normal);
    }

    // Project the vertices of polygon A onto the axes
    for (size_t i = 0; i < axes.size(); i++) {
        Vector2D axis = axes[i];
        float minA = INFINITY, maxA = -INFINITY;
        for (size_t j = 0; j < verticesA.size(); j++) {
            float projection = verticesA[j].dotProduct(axis);
            if (projection < minA) minA = projection;
            if (projection > maxA) maxA = projection;
        }

        // Project the vertices of polygon B onto the axes
        float minB = INFINITY, maxB = -INFINITY;
        for (size_t j = 0; j < verticesB.size(); j++) {
            float projection = verticesB[j].dotProduct(axis);
            if (projection < minB) minB = projection;
            if (projection > maxB) maxB = projection;
        }

        // Check if the projections overlap
        if (maxA < minB || maxB < minA) {
            return false;
        }
    }
    return true;
}












bool Collision::AABB(const ColliderComponent& A, const ColliderComponent& B){
    if (AABB(A.collider, B.collider)){
        // std::cout << A.tag << " hit " << B.tag << std::endl;
        return true;
    }
    else return false;
}