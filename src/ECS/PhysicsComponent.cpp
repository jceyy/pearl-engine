#include "ECS/PhysicsComponent.hpp"
#include "PhysicsManager.hpp"
#include "ECS/Components.hpp"

namespace PRL {

PhysicsComponent::PhysicsComponent() { }

PhysicsComponent::PhysicsComponent(float m, float r, float d, float cor) :
radius(r), mass(m), drag(d), restitutionCoeff(cor) { }


PhysicsComponent::~PhysicsComponent() { }


void PhysicsComponent::computeAcceleration_() {
}


} // namespace PRL