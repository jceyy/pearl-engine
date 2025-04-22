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

    inline bool isStatic() const noexcept { return isStatic_; }
    inline void setStatic(bool isStatic) noexcept { isStatic_ = isStatic; }
    inline void makeStatic() noexcept { isStatic_ = true; }

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
    bool isStatic_;
    bool hasPhysicsComponent_ = false; // To do : remove this variable
};

#endif // _TRANSFORM_COMPONENT_HPP_INCLUDED