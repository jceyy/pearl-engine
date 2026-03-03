#ifndef _ANIMATION_COMPONENT_HPP_INCLUDED
#define _ANIMATION_COMPONENT_HPP_INCLUDED

#include <iostream>
#include <SDL2/SDL.h>
#include "ECS/ECS.hpp"
#include "AssetManager.hpp"

namespace PRL {

class AnimationComponent : public Component {
public:
    AnimationComponent();
    AnimationComponent(const std::string& animName);
    AnimationComponent(const AnimationComponent& other);
    ~AnimationComponent();
    AnimationComponent(AnimationComponent&& other) noexcept = default;
    AnimationComponent& operator=(const AnimationComponent& other) = default;
    AnimationComponent& operator=(AnimationComponent&& other) noexcept = default;

    static inline size_t getInstanceCount() noexcept { return instanceCount_; }

    void inline setHandle(AnimationHandle handle) {
        animHandle = handle;
        currentFrame = 0;
        playing = true;
        runTime = 0;
    }
    
    AnimationHandle animHandle;   // Handle to animation clip
    std::size_t currentFrame; // current frame index
    bool playing;
    uint64_t runTime;      // accumulated time, not sure to keep ?
    
private:
    static size_t instanceCount_;
};

} // namespace PRL

#endif // _ANIMATION_COMPONENT_HPP_INCLUDED