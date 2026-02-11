#include <cassert>
#include "ECS/TransformComponent.hpp"
#include "ECS/PhysicsComponent.hpp"

TransformComponent::TransformComponent() : isStatic_(false) 
{
    position.zero();
    velocity.zero();
    scale = 1.0f;
    w = h = 0;
}

TransformComponent::TransformComponent(PosType x, PosType y) : isStatic_(false){
    velocity.zero();
    position.set(x, y);
    scale = 1.0f;
    w = h = 0;
}

TransformComponent::TransformComponent(PosType x, PosType y, float s) : isStatic_(false) {
    assert(s > 0.0f);
    velocity.zero();
    position.set(x, y);
    scale = s;
    w = h = 0;
}

TransformComponent::TransformComponent(PosType x, PosType y, int w_, int h_, float s) {
    assert(s > 0.0f);
    assert(w_ > 0 and h_ > 0);
    velocity.zero();
    position.set(x, y);
    scale = s;
    w = w_;
    h = h_;
}

void TransformComponent::init() {}

void TransformComponent::setPos(PosType x, PosType y) {
    position.set(x, y);
}

void TransformComponent::update() {
    position += velocity;
}