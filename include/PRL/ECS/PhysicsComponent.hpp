#ifndef _PHYSICS_COMPONENT_HPP_INCLUDED
#define _PHYSICS_COMPONENT_HPP_INCLUDED

#include "ECS.hpp"
#include "TransformComponent.hpp"
#include "../Types.hpp"

namespace PRL {

class PhysicsComponent : public Component {
public:
    PhysicsComponent();
    PhysicsComponent(float mass, float radius, float drag, float cor);
    ~PhysicsComponent();

    float radius = 10;
    float mass = 1;
    float drag = 0;
    float restitutionCoeff = 1;

private:
    Vector2D position_previous_;
    Vector2D acceleration_;

    void computeAcceleration_();
};

} // namespace PRL


#endif // _PHYSICS_COMPONENT_HPP_INCLUDED