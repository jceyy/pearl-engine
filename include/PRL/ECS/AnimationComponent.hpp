#ifndef _ANIMATION_COMPONENT_HPP_INCLUDED
#define _ANIMATION_COMPONENT_HPP_INCLUDED

#include <iostream>
#include <string>
#include <map>
#include <SDL2/SDL.h>
#include "ECS/ECS.hpp"
#include "Animation.hpp"


class AnimationComponent : public Component {
public:
    AnimationComponent();
    AnimationComponent(const AnimationID& animClipID);
    AnimationComponent(const AnimationComponent& other);
    ~AnimationComponent();

    void init() override {}; // to be removed !
    void update() override {}; // to be removed !
    void draw() override {}; // to be removed !

    // float inline getRunTime() const noexcept { return runTime; }
    static inline size_t getInstanceCount() noexcept { return instanceCount_; }

    AnimationID animClipID;   // ID to animation clip
    std::size_t currentFrame; // current frame index
    bool playing;
    float runTime;      // accumulated time, not sure to keep ?
    
private:
    static size_t instanceCount_;
};

#endif // _ANIMATION_COMPONENT_HPP_INCLUDED