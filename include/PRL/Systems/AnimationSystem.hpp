#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "Systems.hpp"
#include "../ECS/SpriteComponent.hpp"

class AnimationSystem : public System {
public:
    AnimationSystem();
    ~AnimationSystem();
    
    void update() override;
    void draw() override;

    inline static size_t instanceCount() noexcept { return instanceCount_; }
private:
    static size_t instanceCount_;
};

#endif // RENDER_SYSTEM_HPP