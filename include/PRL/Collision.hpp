#ifndef COLLISION_HPP
#define COLLISION_HPP

// #include <variant>
#include "Types.hpp"
#include "Geometry.hpp"

namespace PRL {

    namespace Collision {

        class AABB {
        public:
            AABB() = default;
            AABB(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
            ~AABB() = default;

            inline float area() const noexcept { return w * h; }
            inline float perimeter() const noexcept { return 2 * (w + h); }
            inline float centerX() const noexcept { return x + w / 2; }
            inline float centerY() const noexcept { return y + h / 2; }

            const AABB& boundingAABB() const { return *this; }

            float x, y, w, h;
        };


        class Circle {
        public:
            Circle() = default;
            Circle(float x, float y, float r) : x_(x), y_(y), r_(r) {}
            ~Circle() = default;

            inline float area() const noexcept { return M_PI * r_ * r_; }
            inline float perimeter() const noexcept { return 2 * M_PI * r_; }
            inline float centerX() const noexcept { return x_; }
            inline float centerY() const noexcept { return y_; }

            inline float radius() const noexcept { return r_; }
            inline float x() const noexcept { return x_; }
            inline float y() const noexcept { return y_; }
            inline void setRadius(float r) noexcept { r_ = r; aabb_dirty_ = true; }
            inline void setPosition(float x, float y) noexcept { x_ = x; y_ = y; aabb_dirty_ = true; }

            const AABB& boundingAABB() const noexcept {
                if (aabb_dirty_) {
                    computeAABB_();
                    aabb_dirty_ = false;
                }
                return cached_aabb_;
            }
        
        private:
            float x_, y_, r_;
            mutable AABB cached_aabb_;
            mutable bool aabb_dirty_ = true;

            void computeAABB_() const noexcept {
                    cached_aabb_.x = x_ - r_;
                    cached_aabb_.y = y_ - r_;
                    cached_aabb_.w = 2 * r_;
                    cached_aabb_.h = 2 * r_;
            }
        };


        class Capsule {
        public:
            Capsule() = default;
            Capsule(float x, float y, float w, float h) : x_(x), y_(y), w_(w), h_(h) {}
            ~Capsule() = default;

            inline float area() const noexcept { return M_PI * w_ * w_ / 4 + w_ * h_; }
            inline float perimeter() const noexcept { return 2 * M_PI * (w_ / 2) + 2 * h_; }
            inline float centerX() const noexcept { return x_ + w_ / 2; }
            inline float centerY() const noexcept { return y_ + (w_ + h_) / 2; }

            inline float x() const noexcept { return x_; }
            inline float y() const noexcept { return y_; }
            inline float w() const noexcept { return w_; }
            inline float h() const noexcept { return h_; }
            inline void setX(float x) noexcept { x_ = cached_aabb_.x = x; }
            inline void setY(float y) noexcept { y_ = cached_aabb_.y = y; }
            inline void setWidth(float w) noexcept { w_ = w; aabb_dirty_ = true; }
            inline void setHeight(float h) noexcept { h_ = h; aabb_dirty_ = true; }

            const AABB& boundingAABB() const noexcept {
                if (aabb_dirty_) {
                    computeAABB_();
                    aabb_dirty_ = false;
                }
                return cached_aabb_;
            }

        private:
            float x_, y_, w_, h_;
            mutable AABB cached_aabb_;
            mutable bool aabb_dirty_ = true;

            inline void computeAABB_() const noexcept {
                cached_aabb_.w = w_;
                cached_aabb_.h = w_ + h_;
            }
        };


        class ConvexPoly {
        public:
            ConvexPoly() = delete;
            ConvexPoly(const std::vector<Vector2D>& vertices) : vertices(vertices) {
                assert(vertices.size() >= 3 && "ConvexPoly requires at least 3 vertices");
            }
            ~ConvexPoly() = default;

            bool isConvex() const {
                if (vertices.size() < 3) return false;
                bool sign = false;
                for (size_t i = 0; i < vertices.size(); ++i) {
                    const Vector2D& v1 = vertices[i];
                    const Vector2D& v2 = vertices[(i + 1) % vertices.size()];
                    const Vector2D& v3 = vertices[(i + 2) % vertices.size()];
                    float crossProduct = (v2.x - v1.x) * (v3.y - v2.y) - (v2.y - v1.y) * (v3.x - v2.x);
                    if (i == 0) {
                        sign = crossProduct > 0;
                    } else if ((crossProduct > 0) != sign) {
                        return false;
                    }
                }
                return true;
            }

            inline float area() const noexcept {
                float a = 0;
                for (size_t i = 0; i < vertices.size(); ++i) {
                    const Vector2D& v1 = vertices[i];
                    const Vector2D& v2 = vertices[(i + 1) % vertices.size()];
                    a += v1.x * v2.y - v2.x * v1.y;
                }
                return std::abs(a) / 2;
            }
            inline float perimeter() const noexcept {
                float p = 0;
                for (size_t i = 0; i < vertices.size(); ++i) {
                    const Vector2D& v1 = vertices[i];
                    const Vector2D& v2 = vertices[(i + 1) % vertices.size()];
                    p += sqrt((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y));
                }
                return p;
            }

            inline float centerX() const noexcept {
                float cx = 0;
                for (const auto& v : vertices) {
                    cx += v.x;
                }
                return cx / vertices.size();
            }

            inline float centerY() const noexcept {
                float cy = 0;
                for (const auto& v : vertices) {
                    cy += v.y;
                }
                return cy / vertices.size();
            }

            const AABB& boundingAABB() const noexcept {
                // use aabb.x and .y as minx and miny, and
                // aabb.w and .h as maxx and maxy, to avoid needing to store extra state for the AABB
                cached_aabb_.x = vertices[0].x;
                cached_aabb_.w = vertices[0].x;
                cached_aabb_.y = vertices[0].y;
                cached_aabb_.h = vertices[0].y;
                for (size_t i = 1; i < vertices.size(); ++i) {
                    const Vector2D& v = vertices[i];
                    if (v.x < cached_aabb_.x) cached_aabb_.x = v.x;
                    if (v.x > cached_aabb_.w) cached_aabb_.w = v.x;
                    if (v.y < cached_aabb_.y) cached_aabb_.y = v.y;
                    if (v.y > cached_aabb_.h) cached_aabb_.h = v.y;
                }
                cached_aabb_.w -= cached_aabb_.x;
                cached_aabb_.h -= cached_aabb_.y;
                return cached_aabb_;
            }

            std::vector<Vector2D> vertices;

        private:
            mutable AABB cached_aabb_;
            mutable bool aabb_dirty_ = true;

            void computeAABB_() const noexcept {
                float minX = vertices[0].x, maxX = vertices[0].x;
                float minY = vertices[0].y, maxY = vertices[0].y;
                for (size_t i = 1; i < vertices.size(); ++i) {
                    const Vector2D& v = vertices[i];
                    if (v.x < minX)      minX = v.x;
                    else if (v.x > maxX) maxX = v.x;
                    if (v.y < minY)      minY = v.y;
                    else if (v.y > maxY) maxY = v.y;
                }
                cached_aabb_.x = minX;
                cached_aabb_.y = minY;
                cached_aabb_.w = maxX - minX;
                cached_aabb_.h = maxY - minY;
            }
        };



        // Shapes
        enum class ShapeType { AABB, Circle, Capsule, ConvexPolygon };

        struct Shape {
            ShapeType type;
            union {
                Collision::AABB        aabb;
                Collision::Circle      circle;
                Collision::Capsule     capsule;
                Collision::ConvexPoly  poly;
            };
        };




        // Collision algorithms and response flags
        enum class CollisionAlgorithm : uint8_t {
            Discrete,       // compare positions frame-by-frame
            Swept,          // continuous
            Speculative,    // predictive contacts, cheaper than full CCD
            SweptTOI,            // continuous with time of impact resolution, most expensive but most accurate
        };

        enum class CollisionResponseFlags : uint16_t {
            None        = 0,
            Resolve     = 1 << 0,   // apply position correction
            Impulse     = 1 << 1,   // apply velocity response
            Trigger     = 1 << 2,   // fire event, no resolution
            OneShot     = 1 << 3,   // disable after first contact
        };

        // struct CollisionBody {
        //     Shape                   shape;
        //     CollisionAlgorithm      algorithm;
        //     CollisionResponseFlags  response_flags;
        //     LayerID                  layer;          // which layer this belongs to
        //     LayerID                  mask;           // which layers it tests against
        // };


        // Hitboxes
        enum class HitboxRole : uint8_t {
            Hurt,       // "I can receive hits"
            Attack,     // "I deal hits"
            Grab,       // "I can initiate a grab"
            Sense,      // "I detect nearby entities" (aggro, pickup)
        };

        struct Hitbox {
            Shape       shape;
            HitboxRole  role;
            uint32_t    type_flags;     // game-defined: FIRE, POISON, PROJECTILE...
            float       lifetime;       // 0 = permanent
            Vector2D    local_offset;   // relative to entity transform
            // EntityId    owner;          // for damage attribution
        };

        struct HitboxSet {
            static constexpr int MAX = 8;
            Hitbox  boxes[MAX];
            int     count;
        };
    } // namespace Collision










// OLD STUFF
// class ColliderComponent;

// class Collision {
// public:
//     static bool ValueInRange(float value, float min, float max);
//     static bool ValueInRangeStrict(float value, float min, float max);
//     static bool PointLine(const Vector2D& point, const PRL_Line& line);
//     static bool PointAABB(const Vector2D& point, const PRL_Rect& rec);
//     static bool PointCircle(const Vector2D& point, const PRL_Circle& cir);
//     static bool PointPolygon(const Vector2D& point, const PRL_Polygon& poly);
//     static bool Line(const PRL_Line& lineA, const PRL_Line& lineB);
//     static bool LineAABB(const PRL_Line& line, const PRL_Rect& rec);
//     static bool LineCircle(const PRL_Line& line, const PRL_Circle& cir);
//     static bool LinePolygon(const PRL_Line& line, const PRL_Polygon& poly);
//     static bool AABBvsAABB(const PRL_Rect& recA, const PRL_Rect& recB);
//     static bool AABBvsCircle(const PRL_Rect& recA, const PRL_Circle& cirB);
//     static bool CirclevsCircle(const PRL_Circle& cirA, const PRL_Circle& cirB);
//     static bool Polygon(const PRL_Polygon& polyA, const PRL_Polygon& polyB);

//     // static bool AABB(const ColliderComponent& A, const ColliderComponent& B);
// };




// enum class ColliderShapeType {
//     AABB, Circle
// };

// class ColliderShape {
// public:
//     virtual bool checkCollision(const ColliderShape& other) const = 0;
//     virtual ~ColliderShape() = default;
    
//     inline const PRL_Rect& getAABB() const noexcept { return aabb_; }
//     inline ColliderShapeType getType() const noexcept { return type_; }

// protected:
//     virtual void computeAABB_() = 0;
    
//     ColliderShapeType type_;
//     PRL_Rect aabb_; // in world coordinates
// };


// class AABBCollider : public ColliderShape {
// public:
//     AABBCollider(const PRL_Rect& aabb);
//     ~AABBCollider() = default;
    
//     bool checkCollision(const ColliderShape& other) const override;

// private:
//     void computeAABB_() override {} // Does nothing
// };


// class CircleCollider : public ColliderShape {
// public:
//     CircleCollider(const PRL_Circle& circle);
//     ~CircleCollider() = default;

//     bool checkCollision(const ColliderShape& other) const override;
//     inline const PRL_Circle& getCircle() const noexcept { return circle_; }

// private:
//     void computeAABB_() override;
//     PRL_Circle circle_;
// };

} // namespace PRL

#endif // COLLISION_HPP