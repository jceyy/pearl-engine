#include "ColliderComponent.hpp"

ColliderComponent::ColliderComponent() {
    
}

ColliderComponent::ColliderComponent(const std::string& t) : tag(t) {
}

void ColliderComponent::init() {
    entity->addComponent<TransformComponent>();
    transform = &entity->getComponent<TransformComponent>();
    Game::colliders.push_back(this);
}

void ColliderComponent::update() {
    collider.x = transform->position.x;
    collider.y = transform->position.y;
    collider.w = static_cast<int>(transform->w * transform->scale);
    collider.h = static_cast<int>(transform->h * transform->scale);
}