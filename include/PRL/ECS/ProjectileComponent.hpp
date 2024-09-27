#ifndef _PROJECTILE_COMPONENT_HPP_INCLUDED_
#define _PROJECTILE_COMPONENT_HPP_INCLUDED_

#include "ECS.hpp"
#include "TransformComponent.hpp"


class ProjectileComponent : public Component {
public:
    ProjectileComponent(float range, float speed, Vector2D velocity);
    ~ProjectileComponent();

    void init() override;
    void update() override;


private:
    TransformComponent* transform_;
    Vector2D velocity_;
    float range_;
    float speed_;
    float distanceTraveled_;
};


#endif // _PROJECTILE_COMPONENT_HPP_INCLUDED_