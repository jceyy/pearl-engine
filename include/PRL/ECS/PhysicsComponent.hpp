#ifndef _PHYSICS_COMPONENT_HPP_INCLUDED
#define _PHYSICS_COMPONENT_HPP_INCLUDED

#include "ECS.hpp"
#include "TransformComponent.hpp"
#include "../Types.hpp"


class PhysicsComponent : public Component {
public:
    PhysicsComponent();
    PhysicsComponent(float mass, float radius, float drag, float cor);
    ~PhysicsComponent();

    void init() override;
    void update() override;
    void draw() override;
    void postUpdate() {}; // Updates local partition of space localization after constrain have been applied

    Vector2D velocity() const;

    float radius = 10;
    float mass = 1;
    float drag = 0;
    float restitutionCoeff = 1;

private:
    TransformComponent* transform_;
    Vector2D* position_;
    Vector2D position_previous_;
    Vector2D acceleration_;

    void computeAcceleration_();
};


#endif // _PHYSICS_COMPONENT_HPP_INCLUDED