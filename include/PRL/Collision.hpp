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
    static bool AABBvsAABB(const PRL_Rect& recA, const PRL_Rect& recB);
    static bool AABBvsCircle(const PRL_Rect& recA, const PRL_Circle& cirB);
    static bool CirclevsCircle(const PRL_Circle& cirA, const PRL_Circle& cirB);
    static bool Polygon(const PRL_Polygon& polyA, const PRL_Polygon& polyB);

    // static bool AABB(const ColliderComponent& A, const ColliderComponent& B);
};




enum class ColliderShapeType {
    AABB, Circle
};

class ColliderShape {
public:
    virtual bool checkCollision(const ColliderShape& other) const = 0;
    virtual ~ColliderShape() = default;
    
    inline const PRL_Rect& getAABB() const noexcept { return aabb_; }
    inline ColliderShapeType getType() const noexcept { return type_; }

protected:
    virtual void computeAABB_() = 0;
    
    ColliderShapeType type_;
    PRL_Rect aabb_; // in world coordinates
};


class AABBCollider : public ColliderShape {
public:
    AABBCollider(const PRL_Rect& aabb);
    ~AABBCollider() = default;
    
    bool checkCollision(const ColliderShape& other) const override;

private:
    void computeAABB_() override {} // Does nothing
};


class CircleCollider : public ColliderShape {
public:
    CircleCollider(const PRL_Circle& circle);
    ~CircleCollider() = default;

    bool checkCollision(const ColliderShape& other) const override;
    inline const PRL_Circle& getCircle() const noexcept { return circle_; }

private:
    void computeAABB_() override;
    PRL_Circle circle_;
};

#endif // COLLISION_HPP