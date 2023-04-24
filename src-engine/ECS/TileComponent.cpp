#include "TileComponent.hpp"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "../TextureManager.hpp"

TileComponent::TileComponent() {
    texture_ = nullptr;
}

TileComponent::TileComponent(int srcX, int srcY, PosType x, PosType y, int tileSize, float mapScale, const std::string& fileName){
    texture_ = nullptr;

    position.x = x;
    position.y = y;
    fileName_ = fileName;
    srcRect_.x = srcX;
    srcRect_.y = srcY;
    srcRect_.w = srcRect_.h = tileSize;

    dstRect_.x = x;
    dstRect_.y = y;
    dstRect_.w = dstRect_.h = static_cast<int> (tileSize * mapScale);
}


TileComponent::~TileComponent() {
    SDL_DestroyTexture(texture_);
}

void TileComponent::update(){
    dstRect_.x = position.x - Game::camera.x;
    dstRect_.y = position.y - Game::camera.y;
}


void TileComponent::init(){
    if (texture_ != nullptr) SDL_DestroyTexture(texture_);
    texture_ = TextureManager::loadTexture(fileName_);
}

void TileComponent::draw(){
    TextureManager::Draw(texture_, &srcRect_, &dstRect_, SDL_FLIP_NONE);
}