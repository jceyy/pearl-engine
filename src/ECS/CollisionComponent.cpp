#include "ECS/CollisionComponent.hpp"

namespace PRL {

CollisionBodyComponent::CollisionBodyComponent() : 
shape(), algorithm(Collision::CollisionAlgorithm::SAT), response_flags(0), layer(0), mask(0) {
    instanceCount_++;
}

CollisionBodyComponent::CollisionBodyComponent(const CollisionBodyComponent& other) : 
shape(other.shape), algorithm(other.algorithm), response_flags(other.response_flags), layer(other.layer), mask(other.mask) {
    instanceCount_++;

} // namespace PRL