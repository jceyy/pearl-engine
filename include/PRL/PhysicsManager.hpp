#ifndef _PHYSICS_MANAGER_HPP_INCLUDED
#define _PHYSICS_MANAGER_HPP_INCLUDED

#include <iostream>
#include <vector>
#include "Types.hpp"

class Entity;
class PhysicsComponent;

// constexpr float minimumVelocity = 1e-6; 

constexpr std::size_t maxPhysicsInteractions = 4;

class PhysicsManager {
public:
    PhysicsManager();
    ~PhysicsManager();

    void update(); // Not sure this needs to exist
    void applyConstraints();

    static Vector2D globalGravity;
    static float dt;
    static float dt_previous;
    static bool use_const_dt;
    
private:
    friend class PhysicsComponent;
    int constraintPasses_ = 8;
    static std::vector<PhysicsComponent*> physicsComponents;
};


#endif // _PHYSICS_MANAGER_HPP_INCLUDED