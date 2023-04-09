#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <string>
#include "SDL2/SDL.h"
#include "TextureManager.hpp"

class GameObject{

public:
    GameObject(const std::string& textureSheet, SDL_Renderer* renderer, float x0, float y0);
    ~GameObject();

    void update();
    void render();

private:
    float xPos_, yPos_;
    SDL_Rect srcRect_;
    SDL_FRect dstRect_;
    SDL_Texture* texture_;
    SDL_Renderer* renderer_;
};



#endif // GAME_OBJECT_HPP