#include <iostream>
#include "Core.hpp"
#include "ECS/AnimationComponent.hpp"

namespace PRL {

size_t AnimationComponent::instanceCount_ = 0;

AnimationComponent::AnimationComponent() : 
    animHandle(), currentFrame(0), playing(false), runTime(0) {
    instanceCount_++;
}

AnimationComponent::AnimationComponent(const std::string& animName) : 
    animHandle(PRL::Core::getAssetManager().getAnimationHandle(animName)), currentFrame(0), playing(true), runTime(0) {
    instanceCount_++;
}

AnimationComponent::AnimationComponent(const AnimationComponent& other) : 
    animHandle(other.animHandle), currentFrame(other.currentFrame), playing(other.playing), runTime(other.runTime) {
    instanceCount_++;
}

AnimationComponent::~AnimationComponent() {
    instanceCount_--;
}

} // namespace PRL