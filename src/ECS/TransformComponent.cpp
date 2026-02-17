#include <cassert>
#include "ECS/TransformComponent.hpp"

size_t TransformComponent::instanceCount_ = 0;
TransformComponent::TransformComponent() : position(0, 0), scale(1, 1), rotation(0.0) {
    instanceCount_++;
}

TransformComponent::TransformComponent(PosType x, PosType y) : position(x, y), scale(1, 1), rotation(0.0) {
    instanceCount_++;
}

TransformComponent::TransformComponent(PosType x, PosType y, PosType scaleX, PosType scaleY, double rotation) :
position(x, y), scale(scaleX, scaleY), rotation(rotation)
{
    instanceCount_++;
}

TransformComponent::TransformComponent(const TransformComponent& other) :
position(other.position), scale(other.scale), rotation(other.rotation) {
    instanceCount_++;
}

TransformComponent::~TransformComponent() {
    instanceCount_--;
}