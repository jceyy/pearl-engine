#include "Systems/MovementSystem.hpp"
#include "ECS/RigidBodyComponent.hpp"
#include "Core.hpp"

namespace PRL {

size_t MovementSystem::instanceCount_ = 0;

MovementSystem::MovementSystem(ComponentSignature signature) : System(signature) {
    systemName_ = "MovementSystem";
    instanceCount_++;
}

MovementSystem::~MovementSystem() {
    instanceCount_--;
}

void MovementSystem::update() {
    const float dt = static_cast<float>(Core::getFixedDeltaTime());
    if (dt <= 0.f) return;

    const size_t systemID = SystemID::getSystemTypeID<MovementSystem>();
    auto& entities = entityManager_->getEntitiesForSystem(systemID);

    for (size_t i = 0; i < entities.size(); ++i) {
        Entity* entity = entities[i];
        VelocityComponent& vel = entity->getComponent<VelocityComponent>();
        TransformComponent& tf = entity->getComponent<TransformComponent>();

        // Gravity only affects Dynamic rigid bodies; entities with just
        // Transform + Velocity (projectiles, particles...) integrate freely.
        if (entity->hasComponent<RigidBodyComponent>()) {
            const RigidBodyComponent& body = entity->getComponent<RigidBodyComponent>();
            if (body.bodyType == Collision::BodyType::Dynamic) {
                vel.linearVel.x += gravity.x * dt;
                vel.linearVel.y += gravity.y * dt;
                if (vel.linearVel.y > maxFallSpeed) vel.linearVel.y = maxFallSpeed;
            } else if (body.bodyType == Collision::BodyType::Static) {
                continue; // static bodies never move
            }
        }

        tf.position.x += vel.linearVel.x * dt;
        tf.position.y += vel.linearVel.y * dt;
        tf.rotation   += vel.angularVel  * dt;
    }
}

} // namespace PRL
