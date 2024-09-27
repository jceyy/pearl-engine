#include "ECS/PhysicsComponent.hpp"
#include "PhysicsManager.hpp"
#include "ECS/Components.hpp"


PhysicsComponent::PhysicsComponent() { }

PhysicsComponent::PhysicsComponent(float m, float r, float d, float cor) :
radius(r), mass(m), drag(d), restitutionCoeff(cor) { }


PhysicsComponent::~PhysicsComponent() { }


Vector2D PhysicsComponent::velocity() const {
    return (*position_ - position_previous_) / PhysicsManager::dt;
}

void PhysicsComponent::computeAcceleration_() {
    acceleration_ = PhysicsManager::globalGravity;
    transform_->velocity = velocity();

    if (drag != 0)
        acceleration_ -= drag / mass * transform_->velocity;
}

void PhysicsComponent::init() {
    // Requirements
    entity->addComponent<TransformComponent>();
    entity->addComponent<ColliderComponent>();
    entity->addComponent<SpriteComponent>();

    transform_ = &entity->getComponent<TransformComponent>();
    transform_->hasPhysicsComponent_ = true;
    position_ = &transform_->position;
    position_previous_ = *position_;

    PhysicsManager::physicsComponents.push_back(this);
}

void PhysicsComponent::update() {    
    computeAcceleration_();
    Vector2D temp(position_->x, position_->y);

    if (PhysicsManager::use_const_dt) {
        position_->x = position_->x * 2 - position_previous_.x + acceleration_.x 
                    * PhysicsManager::dt * PhysicsManager::dt;
        position_->y = position_->y * 2 - position_previous_.y + acceleration_.y 
                    * PhysicsManager::dt * PhysicsManager::dt;
    }
    else {
        *position_ = *position_ + (*position_ - position_previous_) * PhysicsManager::dt / PhysicsManager::dt_previous
                    + acceleration_ * (PhysicsManager::dt / 2 
                    * (PhysicsManager::dt + PhysicsManager::dt_previous));
    }
    position_previous_ = temp;
}

void PhysicsComponent::draw() {}
