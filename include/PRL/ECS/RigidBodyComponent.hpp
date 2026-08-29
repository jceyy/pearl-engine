#ifndef PRL_RIGID_BODY_COMPONENT_HPP_INCLUDED
#define PRL_RIGID_BODY_COMPONENT_HPP_INCLUDED

#include "../Collision.hpp"
#include "ECS.hpp"
#include "../Types.hpp"

namespace PRL {

class RigidBodyComponent : public Component {
public:
    // RigidBodyComponent();
    RigidBodyComponent();
    explicit RigidBodyComponent(Collision::BodyType type);
    RigidBodyComponent(const RigidBodyComponent&);
    RigidBodyComponent& operator=(const RigidBodyComponent&) = default;
    RigidBodyComponent(RigidBodyComponent&&) noexcept = delete;
    RigidBodyComponent& operator=(RigidBodyComponent&&) noexcept = delete;
    ~RigidBodyComponent();

    Collision::Shape               shape;
    Collision::BodyType            bodyType     = Collision::BodyType::Dynamic;
    Collision::CollisionAlgorithm  algorithm    = Collision::CollisionAlgorithm::Discrete;
    Collision::CollisionResponseFlags responseFlags = Collision::CollisionResponseFlags::Resolve;
    LayerID layer = 0;
    LayerID mask  = 0xFF; // collide against all layers by default
    bool showShape = false; // debug draw

    static inline size_t getInstanceCount() noexcept { return instanceCount_; }

private:
    static size_t instanceCount_;
};

} // namespace PRL

#endif // PRL_RIGID_BODY_COMPONENT_HPP_INCLUDED
