#ifndef POSITION_COMPONENT_HPP
#define POSITION_COMPONENT_HPP

#include "ECS.hpp"
#include "../Types.hpp"
#include "../Vector2D.hpp"

class TransformComponent : public Component {
public:
    TransformComponent();
    TransformComponent(PosType x, PosType y);
    TransformComponent(PosType x, PosType y, float scale);
    TransformComponent(PosType x, PosType y, int w, int h, float scale);

    Vector2D position;
    Vector2D velocity;
    float speed = 3;

    int w;
    int h;
    float scale;

    void init() override;
    void update() override;
    void setPos(PosType x, PosType y);
};

#endif // POSITION_COMPONENT_HPP