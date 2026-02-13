#ifndef _SPRITE_COMPONENT_HPP_INCLUDED
#define _SPRITE_COMPONENT_HPP_INCLUDED

#include <iostream>
#include <string>
#include <map>
#include <SDL2/SDL.h>
#include "Components.hpp"
#include "../Animation.hpp"

class SpriteComponent : public Component {
public:
    SpriteComponent();
    SpriteComponent(const SpriteComponent& other);
    SpriteComponent(SpriteComponent&& other) noexcept;
    SpriteComponent(const std::string& textureID);
    ~SpriteComponent();
    SpriteComponent& operator=(const SpriteComponent& other);
    SpriteComponent& operator=(SpriteComponent&& other) noexcept;

    void init() override;
    void update() override {}; // to be removed !
    void draw() override {}; // to be removed !
    void setTexture(const std::string& textureID);

    static inline size_t getInstanceCount() noexcept { return instanceCount_; }
    
    SDL_RendererFlip spriteFlip;

private:
    void updateRect_(int w, int h);

    std::string textureID_;
    SDL_Texture* texture_;
    SDL_Rect srcRect_;
    SDL_FRect dstRect_;

    static size_t instanceCount_;

    friend class RenderSystem;
};

#endif // _SPRITE_COMPONENT_HPP_INCLUDED