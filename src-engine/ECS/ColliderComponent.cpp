#include "ColliderComponent.hpp"
#include "../TextureManager.hpp"

ColliderComponent::ColliderComponent() {
    
}

ColliderComponent::ColliderComponent(const std::string& t) : tag(t) {
}

ColliderComponent::ColliderComponent(const std::string& t, PosType x, PosType y, int size) : tag(t) {
    collider.x = x;
    collider.y = y;
    collider.h = collider.w = size;
}


void ColliderComponent::init() {
    entity->addComponent<TransformComponent>();
    transform = &entity->getComponent<TransformComponent>();

    texture_ = TextureManager::loadTexture("assets/ColTex.png");
    srcRect_ = {0, 0, 32, 32};
    dstRect_ = {collider.x, collider.y, collider.w, collider.h};
}

void ColliderComponent::update() {
    if (tag != "terrain"){
        collider.x = transform->position.x;
        collider.y = transform->position.y;
        collider.w = static_cast<int>(transform->w * transform->scale);
        collider.h = static_cast<int>(transform->h * transform->scale);
    }
    dstRect_.x = collider.x - Game::camera.x;
    dstRect_.y = collider.y - Game::camera.y;
}

void ColliderComponent::draw() {
    TextureManager::Draw(texture_, &srcRect_, &dstRect_, SDL_FLIP_NONE);
}