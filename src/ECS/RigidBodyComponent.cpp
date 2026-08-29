#include "ECS/RigidBodyComponent.hpp"

namespace PRL {

size_t RigidBodyComponent::instanceCount_ = 0;

RigidBodyComponent::RigidBodyComponent() :
    shape(),
    bodyType(Collision::BodyType::Dynamic),
    algorithm(Collision::CollisionAlgorithm::Discrete),
    responseFlags(Collision::CollisionResponseFlags::Resolve),
    layer(0), mask(0xFF), showShape(false) {
    instanceCount_++;
}

RigidBodyComponent::RigidBodyComponent(Collision::BodyType type) :
    shape(),
    bodyType(type),
    algorithm(Collision::CollisionAlgorithm::Discrete),
    responseFlags(Collision::CollisionResponseFlags::Resolve),
    layer(0), mask(0xFF), showShape(false) {
    instanceCount_++;
}

RigidBodyComponent::RigidBodyComponent(const RigidBodyComponent& other) :
    shape(other.shape),
    bodyType(other.bodyType),
    algorithm(other.algorithm),
    responseFlags(other.responseFlags),
    layer(other.layer), mask(other.mask),
    showShape(other.showShape) {
    instanceCount_++;
}

RigidBodyComponent::~RigidBodyComponent() {
    instanceCount_--;
}

} // namespace PRL
