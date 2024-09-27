#include "ECS/ProjectileComponent.hpp"
#include "Game.hpp"


ProjectileComponent::ProjectileComponent(float range, float speed, Vector2D vel) :
transform_(nullptr), velocity_(vel), range_(range), speed_(speed), distanceTraveled_(0){
    if (range_ < 0) {
        range *= -1;
    }
}

ProjectileComponent::~ProjectileComponent() {}

void ProjectileComponent::init(){
    transform_ = &entity->getComponent<TransformComponent>();
    transform_->velocity = velocity_;
}


void ProjectileComponent::update(){
    distanceTraveled_ += speed_;
    if (distanceTraveled_ > range_) {
        entity->destroy();
    }
    else if (transform_->position.x > Game::camera.x + Game::camera.w ||
             transform_->position.y > Game::camera.y + Game::camera.h ||
             transform_->position.x < Game::camera.x ||
             transform_->position.y < Game::camera.y){
                entity->destroy();
             }
}