#ifndef _ANIMATION_COMPONENT_HPP_INCLUDED
#define _ANIMATION_COMPONENT_HPP_INCLUDED

#include <iostream>
#include <string>
#include <map>
#include <SDL2/SDL.h>
#include "ECS/ECS.hpp"
#include "../Animation.hpp"

class AnimationComponent : public Component {
public:
    AnimationComponent();
    AnimationComponent(const AnimationComponent& other);
    AnimationComponent(AnimationComponent&& other) noexcept = default;
    AnimationComponent(const std::string& textureID);
    ~AnimationComponent();
    AnimationComponent& operator=(const AnimationComponent& other) = default;
    AnimationComponent& operator=(AnimationComponent&& other) noexcept = default;
    void init() override;
    // void setAnimation(const Animation& animation);
    // inline Animation getAnimation() const noexcept;

private:
    bool animated_;
    size_t animFrames_;
    size_t animFPS_;

    size_t animIndex_;
    std::map<std::string, Animation> animations_;

    static size_t count_;
    // friend class RenderSystem;
};

#endif // _ANIMATION_COMPONENT_HPP_INCLUDED