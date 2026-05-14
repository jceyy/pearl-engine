#ifndef PRL_COLLISION_COMPONENT_HPP_INCLUDED
#define PRL_COLLISION_COMPONENT_HPP_INCLUDED

#include <SDL2/SDL.h>
#include <string>
#include "../Types.hpp"
#include "ECS.hpp"
#include "Collision.hpp"

// enum class ColliderType {
//     Player, Enemy, Terrain, Projectile, Tile, Button, Label
// };

// To do : Also FrameColliderComponent, which is a collider that is not updated every frame

namespace PRL {

    class CollisionBodyComponent : public Component {
    public:
        CollisionBodyComponent();
        CollisionBodyComponent(const CollisionBodyComponent&);
        CollisionBodyComponent& operator=(const CollisionBodyComponent&) = default;
        CollisionBodyComponent(CollisionBodyComponent&&) noexcept;
        CollisionBodyComponent& operator=(CollisionBodyComponent&&) noexcept;
        ~CollisionBodyComponent();

        Collision::Shape                   shape;
        Collision::CollisionAlgorithm      algorithm;
        Collision::CollisionResponseFlags  response_flags;
        LayerID layer; // which layer this belongs to
        LayerID mask; // which layers it tests against
        bool showCollider = false; // for debug purposes

        static inline size_t getInstanceCount() noexcept { return instanceCount_; }

    private:
        static size_t instanceCount_;
    };

} // namespace PRL

#endif // PRL_COLLISION_COMPONENT_HPP_INCLUDED