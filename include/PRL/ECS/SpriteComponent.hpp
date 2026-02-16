#ifndef _SPRITE_COMPONENT_HPP_INCLUDED
#define _SPRITE_COMPONENT_HPP_INCLUDED

#include <iostream>
#include <SDL2/SDL.h>
#include "Components.hpp"

class SpriteComponent : public Component {
public:
    SpriteComponent();
    SpriteComponent(const TextureID& textureID);
    SpriteComponent(const SpriteComponent& other);
    ~SpriteComponent();
    SpriteComponent(SpriteComponent&& other) noexcept = default;
    SpriteComponent& operator=(const SpriteComponent& other) = default;
    SpriteComponent& operator=(SpriteComponent&& other) noexcept = default;

    void init() override {}; // to be removed !
    void update() override {}; // to be removed !
    void draw() override {}; // to be removed !

    static inline size_t getInstanceCount() noexcept { return instanceCount_; }
    
    TextureID textureID;
    size_t layer;
    size_t region;
    SDL_RendererFlip spriteFlip;
    bool visible;

private:
    static size_t instanceCount_;
};

#endif // _SPRITE_COMPONENT_HPP_INCLUDED