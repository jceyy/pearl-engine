#include "Systems/AnimationSystem.hpp"
#include "ECS/AnimationComponent.hpp"

size_t AnimationSystem::instanceCount_ = 0;

AnimationSystem::AnimationSystem() {
    signature_.set(getComponentTypeID<AnimationComponent>());
    instanceCount_++;
}

AnimationSystem::~AnimationSystem() {
    instanceCount_--;
}