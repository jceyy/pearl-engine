#ifndef ANIMATION_SYSTEM_HPP
#define ANIMATION_SYSTEM_HPP

#include "Systems.hpp"
#include "ECS/AnimationComponent.hpp"
#include "ECS/SpriteComponent.hpp"

namespace PRL {

class AnimationSystem : public System {
public:
    AnimationSystem(ComponentSignature signature = 
        ComponentSignature::create<AnimationComponent, SpriteComponent>());
    ~AnimationSystem();
    
    void update() override;
    void draw() override {};

    inline void setDeltaTime(uint64_t deltaTimeUS) noexcept { deltaTimeUS_ = deltaTimeUS; }

    inline static size_t instanceCount() noexcept { return instanceCount_; }

private:
    uint64_t deltaTimeUS_;
    static size_t instanceCount_;
};

} // namespace PRL

#endif // ANIMATION_SYSTEM_HPP