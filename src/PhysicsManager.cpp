#include "PhysicsManager.hpp"
#include "Constants.hpp"


Vector2D PhysicsManager::globalGravity = Vector2D(0, PHYSICS_g_earth);
float PhysicsManager::dt = 5e-2;
float PhysicsManager::dt_previous = 1e-2;
bool PhysicsManager::use_const_dt = true;

std::vector<PhysicsComponent*> PhysicsManager::physicsComponents;

PhysicsManager::PhysicsManager() {}

PhysicsManager::~PhysicsManager() {}

void PhysicsManager::update() {}

void PhysicsManager::applyConstraints() {
    return;
}

