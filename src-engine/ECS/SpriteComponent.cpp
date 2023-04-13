#include "SpriteComponent.hpp"
#include "../TextureManager.hpp"

SpriteComponent::SpriteComponent() : transform_(nullptr), texture_(nullptr),
srcRect_({0, 0, 0, 0}), dstRect_({0.0f, 0.0f, 0, 0}){
}

SpriteComponent::~SpriteComponent() {
    SDL_DestroyTexture(texture_);
}

void SpriteComponent::init() {
    entity->addComponent<TransformComponent>();
    transform_ = &entity->getComponent<TransformComponent>();

    srcRect_.x = srcRect_.y = 0;
    srcRect_.w = srcRect_.h = 0;
    dstRect_.x = dstRect_.x = 0.0f;
    dstRect_.w = dstRect_.h = 64;
}

void SpriteComponent::update() {
    dstRect_.x = transform_->position.x;
    dstRect_.y = transform_->position.y;
    dstRect_.w = transform_->w * transform_->scale;
    dstRect_.h = transform_->h * transform_->scale;
}

void SpriteComponent::draw() {
    TextureManager::Draw(texture_, &srcRect_, &dstRect_);
}

void SpriteComponent::setTexture(const std::string& fileName) {
    int w(0), h(0);
    texture_ = TextureManager::loadTexture(fileName, w, h);

    transform_->w = w;
    transform_->h = h;

    srcRect_.w = w;
    srcRect_.h = h;
    dstRect_.w = static_cast<int>(transform_->scale * w);
    dstRect_.h = static_cast<int>(transform_->scale * h); 
}