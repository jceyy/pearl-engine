#include "ECS/TileComponent.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/SpriteComponent.hpp"
#include "TextureManager.hpp"
#include "Game.hpp"

using namespace PRL;

size_t TileMapComponent::instanceCount_ = 0;



TileComponent::TileComponent() {
    texture_ = nullptr;
}

TileComponent::TileComponent(int srcX, int srcY, PosType x, PosType y, int tileSize, float mapScale, const std::string& texID) {
    texture_ = nullptr;

    position.x = x;
    position.y = y;
    textureID_ = texID;
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
    dstRect_.x = static_cast<PosType>(position.x - PRL::Game::camera.x);
    dstRect_.y = static_cast<PosType>(position.y - PRL::Game::camera.y);
}


void TileComponent::init(){
    if (texture_ != nullptr) SDL_DestroyTexture(texture_);
    // auto texHandle = Game::assetManager->getTextureHandle(textureID_);
    // texture_ = Game::assetManager->getTexture(texHandle);
    texture_ = nullptr;
    // To be replaced soon
    PRL::Logging::log("To be deprecated soon", "TileComponent::init");

}

void TileComponent::draw(){
    TextureManager::Draw(texture_, &srcRect_, &dstRect_, SDL_FLIP_NONE);
}