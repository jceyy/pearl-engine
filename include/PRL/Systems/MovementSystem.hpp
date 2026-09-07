#ifndef PRL_MOVEMENT_SYSTEM_HPP_INCLUDED
#define PRL_MOVEMENT_SYSTEM_HPP_INCLUDED

#include "Systems.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/VelocityComponent.hpp"
#include "Vector2D.hpp"

namespace PRL {

//! \brief Integrates VelocityComponent into TransformComponent once per fixed
//! update, and applies gravity to entities with a Dynamic RigidBodyComponent.
//!
//! Uses Core::getFixedDeltaTime() as the timestep — the outer loop is expected
//! to call SystemManager::update() at a fixed rate (see apps/main.cpp).
//! Register this system BEFORE CollisionSystem so that penetrations caused by
//! integration are resolved in the same fixed step.
class MovementSystem : public System {
public:
    explicit MovementSystem(ComponentSignature signature =
        ComponentSignature::create<TransformComponent, VelocityComponent>());
    ~MovementSystem();

    //! \brief World units / s². +y points down (screen convention).
    Vector2D gravity = Vector2D(0.f, 40.f);

    //! \brief Clamp on downward velocity (world units / s) for Dynamic bodies.
    float maxFallSpeed = 30.f;

    void update() override;
    void draw() override {}

    static inline size_t getInstanceCount() noexcept { return instanceCount_; }

private:
    static size_t instanceCount_;
};

} // namespace PRL

#endif // PRL_MOVEMENT_SYSTEM_HPP_INCLUDED
