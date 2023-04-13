#include <iostream>
#include "GameObject.hpp"

GameObject::GameObject(const std::string& textureSheet, SDL_Renderer* renderer, float x0, float y0) : 
xPos_(x0), yPos_(y0), renderer_(renderer) {
    texture_= TextureManager::loadTexture(textureSheet);
}

void GameObject::update(){
    xPos_++;
    yPos_++;

    srcRect_.x = 0;
    srcRect_.y = 0;
    srcRect_.w = 1300;
    srcRect_.h = 1600;

    dstRect_.x = xPos_;
    dstRect_.y = yPos_;
    dstRect_.w = srcRect_.w/10;
    dstRect_.h = srcRect_.h/10;

}

void GameObject::render(){
    SDL_RenderCopyF(renderer_, texture_, &srcRect_, &dstRect_);
}