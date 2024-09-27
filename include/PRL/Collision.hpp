#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Types.hpp"
#include "Geometry.hpp"

class ColliderComponent;

class Collision {
public:
    static bool ValueInRange(float value, float min, float max);
    static bool ValueInRangeStrict(float value, float min, float max);
    static bool PointLine(const Vector2D& point, const PRL_Line& line);
    static bool PointAABB(const Vector2D& point, const PRL_Rect& rec);
    static bool PointCircle(const Vector2D& point, const PRL_Circle& cir);
    static bool PointPolygon(const Vector2D& point, const PRL_Polygon& poly);
    static bool Line(const PRL_Line& lineA, const PRL_Line& lineB);
    static bool LineAABB(const PRL_Line& line, const PRL_Rect& rec);
    static bool LineCircle(const PRL_Line& line, const PRL_Circle& cir);
    static bool LinePolygon(const PRL_Line& line, const PRL_Polygon& poly);
    static bool AABB(const PRL_Rect& recA, const PRL_Rect& recB);
    static bool AABBCircle(const PRL_Rect& recA, const PRL_Circle& cirB);
    static bool Circle(const PRL_Circle& cirA, const PRL_Circle& cirB);
    static bool Polygon(const PRL_Polygon& polyA, const PRL_Polygon& polyB);

    static bool AABB(const ColliderComponent& A, const ColliderComponent& B);

private:
    // static PRL_Rect rect;
};


#endif // COLLISION_HPP