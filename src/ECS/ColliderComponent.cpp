#include "ECS/ColliderComponent.hpp"
#include "TextureManager.hpp"

// ColliderComponent::ColliderComponent() : 
//     isTrigger_(false), tag_(""), shape_(nullptr), transform_(nullptr) {}

ColliderComponent::ColliderComponent(const std::string& tag, const ColliderShape& shape, bool isTrigger) : 
    isTrigger_(isTrigger), tag_(tag), transform_(nullptr) {
    if (const AABBCollider* aabb = dynamic_cast<const AABBCollider*>(&shape)) {
        shape_ = static_cast<ColliderShape*>(new AABBCollider(aabb->getAABB()));
    }
    else if (const CircleCollider* circle = dynamic_cast<const CircleCollider*>(&shape)) {
        shape_ = static_cast<ColliderShape*>(new CircleCollider(circle->getCircle()));
    }
    else {
        throw std::runtime_error("Unknown collider shape type");
    }
}

ColliderComponent::~ColliderComponent() {
    if (shape_ != nullptr) {
        delete shape_;
    }
}

void ColliderComponent::init() {
    entity->addComponent<TransformComponent>();
    transform_ = &entity->getComponent<TransformComponent>();
}

void ColliderComponent::update() {
    // if (tag_ != "terrain"){
    //     collider.x = transform->position.x;
    //     collider.y = transform->position.y;
    //     collider.w = static_cast<int>(transform->w * transform->scale);
    //     collider.h = static_cast<int>(transform->h * transform->scale);
    // }
    // dstRect_.x = collider.x - Game::camera.x;
    // dstRect_.y = collider.y - Game::camera.y;
}

void ColliderComponent::draw() {}