#include "ECS/TransformComponent.hpp"
#include "ECS/PhysicsComponent.hpp"

TransformComponent::TransformComponent() : hasPhysicsComponent_(false) 
{
    position.zero();
    velocity.zero();
    scale = 1.0f;
    w = h = 0;
}

TransformComponent::TransformComponent(PosType x, PosType y) : hasPhysicsComponent_(false){
    velocity.zero();
    position.set(x, y);
    scale = 1.0f;
    w = h = 0;
}

TransformComponent::TransformComponent(PosType x, PosType y, float s) : hasPhysicsComponent_(false){
    velocity.zero();
    position.set(x, y);
    scale = s;
    w = h = 0;
}

TransformComponent::TransformComponent(PosType x, PosType y, int w_, int h_, float s) {
    velocity.zero();
    position.set(x, y);
    scale = s;
    w = w_;
    h = h_;
}

void TransformComponent::init() {
    if (this->entity->hasComponent<PhysicsComponent>()) {
        hasPhysicsComponent_ = true;
    }
}

void TransformComponent::setPos(PosType x, PosType y) {
    position.set(x, y);
}

void TransformComponent::update() {
    if (not hasPhysicsComponent_)
        position += velocity;
}