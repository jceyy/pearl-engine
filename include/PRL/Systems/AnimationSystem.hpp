#ifndef ANIMATION_SYSTEM_HPP
#define ANIMATION_SYSTEM_HPP

#include "Systems.hpp"
#include "ECS/AnimationComponent.hpp"
#include "ECS/SpriteComponent.hpp"

class AnimationSystem : public System {
public:
    AnimationSystem(ComponentSignature signature = 
        ComponentSignature::create<AnimationComponent, SpriteComponent>());
    ~AnimationSystem();
    
    void update() override;
    void draw() override {};

    inline static size_t instanceCount() noexcept { return instanceCount_; }

private:
    float deltaTime_;
    static size_t instanceCount_;
};

#endif // ANIMATION_SYSTEM_HPP