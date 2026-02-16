#ifndef _TRANSFORM_COMPONENT_HPP_INCLUDED
#define _TRANSFORM_COMPONENT_HPP_INCLUDED

#include "ECS.hpp"
#include "Types.hpp"
#include "Vector2D.hpp"

class TransformComponent : public Component {
public:
    TransformComponent();
    TransformComponent(PosType x, PosType y);
    TransformComponent(PosType x, PosType y, PosType scaleX = 1.0, 
        PosType scaleY = 1.0, double rotation = 0.0);
    TransformComponent(const TransformComponent& other);
    TransformComponent(TransformComponent&& other) noexcept = default;
    ~TransformComponent();
    TransformComponent& operator=(const TransformComponent& other) = default;
    TransformComponent& operator=(TransformComponent&& other) noexcept = default;

    Vector2D position;
    Vector2D scale;
    double rotation;

    inline void setPos(PosType x, PosType y) noexcept { position.set(x, y); };

    inline static size_t getInstanceCount() noexcept { return instanceCount_; }

private:
    static size_t instanceCount_;
};

#endif // _TRANSFORM_COMPONENT_HPP_INCLUDED
