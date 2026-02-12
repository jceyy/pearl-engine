#include "Systems/AnimationSystem.hpp"

size_t AnimationSystem::instanceCount_ = 0;

AnimationSystem::AnimationSystem(EntityManager* entityManager, ComponentSignature signature) :
System(entityManager, signature) {
    signature_.set(ComponentID::getComponentTypeID<AnimationComponent>());
    instanceCount_++;
}

AnimationSystem::~AnimationSystem() {
    instanceCount_--;
}