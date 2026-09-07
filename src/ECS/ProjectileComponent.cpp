#include "ECS/ProjectileComponent.hpp"
#include "Core.hpp"

namespace PRL {

ProjectileComponent::ProjectileComponent(float range, float speed, Vector2D vel) :
transform_(nullptr), velocity_(vel), range_(range), speed_(speed), distanceTraveled_(0){
    if (range_ < 0) {
        range *= -1;
    }
}

ProjectileComponent::~ProjectileComponent() {}

void ProjectileComponent::init(){
    transform_ = &entity->getComponent<TransformComponent>();
    // transform_->velocity = velocity_;
}


void ProjectileComponent::update(){
    distanceTraveled_ += speed_;
    if (distanceTraveled_ > range_) {
        entity->destroy();
    }
    else {
        // Destroy projectiles that leave the visible world-space view (small margin).
        const Collision::AABB view = Core::getCamera().viewAABB();
        const float margin = 2.f; // world units
        if (transform_->position.x > view.x + view.w + margin ||
            transform_->position.y > view.y + view.h + margin ||
            transform_->position.x < view.x - margin ||
            transform_->position.y < view.y - margin){
                entity->destroy();
        }
    }
}

} // namespace PRL
