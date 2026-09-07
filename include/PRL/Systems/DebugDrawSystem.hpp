#ifndef PRL_DEBUG_DRAW_SYSTEM_HPP_INCLUDED
#define PRL_DEBUG_DRAW_SYSTEM_HPP_INCLUDED

#include "Systems.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/RigidBodyComponent.hpp"

namespace PRL {

class CollisionSystem;

//! \brief Draws collision debug overlays: world-space AABBs of every rigid body
//! and of the solid tiles of the tilemaps registered with the CollisionSystem.
//!
//! IMPORTANT: register this system LAST. Draw order follows registration order,
//! so registering it last guarantees the overlays paint on top of tiles and
//! sprites (this ordering was why drawing from CollisionSystem::draw() — which
//! runs second — never showed anything).
//!
//! Globally toggled via setEnabled()/toggle(), e.g. bound to 'c' in Game.
class DebugDrawSystem : public System {
public:
    explicit DebugDrawSystem(ComponentSignature signature =
        ComponentSignature::create<TransformComponent, RigidBodyComponent>());
    ~DebugDrawSystem();

    //! \brief Source of the registered tilemaps to overlay.
    void setCollisionSystem(const CollisionSystem* system) noexcept { collisionSystem_ = system; }

    static void setEnabled(bool enabled) noexcept { enabled_ = enabled; }
    static bool isEnabled() noexcept { return enabled_; }
    static void toggle() noexcept { enabled_ = !enabled_; }

    void update() override {}
    void draw() override;

    static inline size_t getInstanceCount() noexcept { return instanceCount_; }

private:
    const CollisionSystem* collisionSystem_ = nullptr;

    static bool enabled_;
    static size_t instanceCount_;
};

} // namespace PRL

#endif // PRL_DEBUG_DRAW_SYSTEM_HPP_INCLUDED
