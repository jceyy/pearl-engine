#ifndef _TRANSFORM_COMPONENT_HPP_INCLUDED
#define _TRANSFORM_COMPONENT_HPP_INCLUDED

#include "ECS.hpp"
#include "Types.hpp"
#include "Vector2D.hpp"

class VelocityComponent : public Component {
public:
    VelocityComponent();
    VelocityComponent(PosType vx, PosType vy, PosType angularVel = 0.0f);
    VelocityComponent(const VelocityComponent& other);
    VelocityComponent(VelocityComponent&& other) noexcept = default;
    ~VelocityComponent();
    VelocityComponent& operator=(const VelocityComponent& other) = default;
    VelocityComponent& operator=(VelocityComponent&& other) noexcept = default;

    Vector2D linearVel;
    float angularVel;

    void init() override {}
    inline void setVel(PosType x, PosType y) noexcept { linearVel.set(x, y); };

    inline static size_t getInstanceCount() noexcept { return instanceCount_; }

private:
    static size_t instanceCount_;
};

#endif // _TRANSFORM_COMPONENT_HPP_INCLUDED