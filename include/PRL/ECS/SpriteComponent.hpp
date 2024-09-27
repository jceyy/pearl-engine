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
    SpriteComponent(const std::string& textureID, bool animated);
    ~SpriteComponent();

    void init() override;
    void update() override;
    void draw() override;
    void setTexture(const std::string& textureID);
    void setTexture(const std::string& textureID, int w, int h);
    void play(const std::string& animName);
    void setFlip(SDL_RendererFlip flip);
    SDL_RendererFlip getFlip() const;


private:
    void updateRect_(int w, int h);

    TransformComponent* transform_;
    std::string textureID_;
    SDL_Texture* texture_;
    SDL_Rect srcRect_;
    SDL_FRect dstRect_;
    SDL_RendererFlip spriteFlip_;
    bool animated_;
    size_t animFrames_;
    size_t animFPS_;

    size_t animIndex_;
    std::map<std::string, Animation> animations_;
};

#endif // _SPRITE_COMPONENT_HPP_INCLUDED