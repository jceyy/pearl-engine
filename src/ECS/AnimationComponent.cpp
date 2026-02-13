#include <iostream>
#include "ECS/AnimationComponent.hpp"

size_t AnimationComponent::instanceCount_ = 0;

AnimationComponent::AnimationComponent() {
    instanceCount_++;
}

AnimationComponent::AnimationComponent(const AnimationComponent& other) {
    instanceCount_++;
}

AnimationComponent::AnimationComponent(const std::string& textureID) {
    instanceCount_++;
}

AnimationComponent::~AnimationComponent() {
    instanceCount_--;
}

void AnimationComponent::init() {
    // Initialization code here
}
