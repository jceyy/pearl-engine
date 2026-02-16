#include <iostream>
#include "ECS/AnimationComponent.hpp"

size_t AnimationComponent::instanceCount_ = 0;

AnimationComponent::AnimationComponent(const AnimationComponent& other) : 
    animClipID(other.animClipID), currentFrame(other.currentFrame), playing(other.playing), runTime(other.runTime) {
    instanceCount_++;
}

AnimationComponent::AnimationComponent(const AnimationID& animClipID) : 
    animClipID(animClipID), currentFrame(0), playing(false), runTime(0.0f) {
    instanceCount_++;
}

AnimationComponent::~AnimationComponent() {
    instanceCount_--;
}
