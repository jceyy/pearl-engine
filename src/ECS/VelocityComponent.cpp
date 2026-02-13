#include "ECS/VelocityComponent.hpp"

size_t VelocityComponent::instanceCount_ = 0;

VelocityComponent::VelocityComponent() : linearVel(0, 0), angularVel(0) {
    instanceCount_++;
}

VelocityComponent::VelocityComponent(PosType vx, PosType vy, PosType angularVel) : linearVel(vx, vy), angularVel(angularVel) {
    instanceCount_++;
}

VelocityComponent::VelocityComponent(const VelocityComponent& other) : linearVel(other.linearVel), angularVel(other.angularVel) {
    instanceCount_++;
}

VelocityComponent::~VelocityComponent() {
    instanceCount_--;
}