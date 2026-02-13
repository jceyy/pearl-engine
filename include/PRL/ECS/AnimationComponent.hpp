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
    void update() override {}; // to be removed !
    void draw() override {}; // to be removed !

    // void setAnimation(const Animation& animation);
    // inline Animation getAnimation() const noexcept;

    static inline size_t getInstanceCount() noexcept { return instanceCount_; }

private:
    size_t animFrames_;
    size_t animFPS_;

    size_t animIndex_;
    std::map<std::string, Animation> animations_;

    static size_t instanceCount_;
};

#endif // _ANIMATION_COMPONENT_HPP_INCLUDED