#ifndef ANIMATION_SYSTEM_HPP
#define ANIMATION_SYSTEM_HPP

#include "Systems.hpp"
#include "ECS/AnimationComponent.hpp"

class AnimationSystem : public System {
public:
    AnimationSystem(EntityManager* entityManager, ComponentSignature signature = ComponentSignature::create<AnimationComponent>());
    ~AnimationSystem();
    
    void update() override;
    void draw() override;

    inline static size_t instanceCount() noexcept { return instanceCount_; }

private:
    static size_t instanceCount_;
};

#endif // ANIMATION_SYSTEM_HPP