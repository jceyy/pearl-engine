#include "TransformComponent.hpp"

TransformComponent::TransformComponent() 
{
    position.zero();
    velocity.zero();
    scale = 1.0f;
    w = h = 0;
}

TransformComponent::TransformComponent(PosType x, PosType y){
    velocity.zero();
    position.set(x, y);
    scale = 1.0f;
    w = h = 0;
}

TransformComponent::TransformComponent(PosType x, PosType y, float s){
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
    velocity.zero();
}

void TransformComponent::setPos(PosType x, PosType y) {
    position.set(x, y);
}

void TransformComponent::update() {
    position += velocity * speed;
}