#ifndef _TRANSFORM_COMPONENT_HPP_INCLUDED
#define _TRANSFORM_COMPONENT_HPP_INCLUDED

#include "ECS.hpp"
#include "../Types.hpp"
#include "../Vector2D.hpp"

class TransformComponent : public Component {
public:
    TransformComponent();
    TransformComponent(PosType x, PosType y);
    TransformComponent(PosType x, PosType y, float scale);
    TransformComponent(PosType x, PosType y, int w, int h, float scale);

    bool isStatic() const;
    void switchStatic();

    Vector2D position;
    Vector2D velocity;

    int w;
    int h;
    float scale;

    void init() override;
    void update() override;
    void setPos(PosType x, PosType y);


private:
    friend class PhysicsComponent;
    bool hasPhysicsComponent_;
    bool isStatic_;
};

#endif // _TRANSFORM_COMPONENT_HPP_INCLUDED