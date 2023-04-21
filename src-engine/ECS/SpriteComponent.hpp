#ifndef SPRITE_COMPONENT_HPP
#define SPRITE_COMPONENT_HPP

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include "Components.hpp"

class SpriteComponent : public Component {
public:
    SpriteComponent();
    ~SpriteComponent();

    void init() override;
    void update() override;
    void draw() override;
    void setTexture(const std::string& fileName);
    void setTexture(const std::string& fileName, int w, int h);

private:
    void updateRect_(int w, int h);
    TransformComponent* transform_;
    SDL_Texture* texture_;
    SDL_Rect srcRect_;
    SDL_FRect dstRect_;
};

#endif // SPRITE_COMPONENT_HPP