#include "ECS/SpriteComponent.hpp"
#include "TextureManager.hpp"

size_t SpriteComponent::instanceCount_ = 0;

// Constructors, destructor, and assignment operators
SpriteComponent::SpriteComponent() :
textureID_(""), texture_(nullptr), 
srcRect_({0, 0, 0, 0}), dstRect_({0, 0, 0, 0}), spriteFlip(SDL_FLIP_NONE) {
    instanceCount_++;
}

SpriteComponent::SpriteComponent(const SpriteComponent& other) :
textureID_(other.textureID_), texture_(other.texture_), 
srcRect_(other.srcRect_), dstRect_(other.dstRect_), spriteFlip(other.spriteFlip) {
    instanceCount_++;
}

SpriteComponent::SpriteComponent(SpriteComponent&& other) noexcept :
textureID_(std::move(other.textureID_)), texture_(other.texture_),
srcRect_(other.srcRect_), dstRect_(other.dstRect_), spriteFlip(other.spriteFlip) {
    other.texture_ = nullptr;
}

SpriteComponent::SpriteComponent(const std::string& textureID) :
textureID_(textureID), texture_(nullptr), 
srcRect_({0, 0, 0, 0}), dstRect_({0, 0, 0, 0}), spriteFlip(SDL_FLIP_NONE) {
    instanceCount_++;
}

SpriteComponent::~SpriteComponent() {
    instanceCount_--;
}

SpriteComponent& SpriteComponent::operator=(const SpriteComponent& other) {
    if (this != &other) {
        textureID_ = other.textureID_;
        texture_ = other.texture_;
        srcRect_ = other.srcRect_;
        dstRect_ = other.dstRect_;
        spriteFlip = other.spriteFlip;
    }
    return *this;
}

SpriteComponent& SpriteComponent::operator=(SpriteComponent&& other) noexcept {
    if (this != &other) {
        spriteFlip = other.spriteFlip;
        textureID_ = std::move(other.textureID_);
        texture_ = other.texture_;
        srcRect_ = other.srcRect_;
        dstRect_ = other.dstRect_;
        other.texture_ = nullptr;
    }
    return *this;
}


void SpriteComponent::init() {
    if (textureID_ != std::string("")) {
        setTexture(textureID_);
    }
}

void SpriteComponent::setTexture(const std::string& texID) {
    int w(0), h(0);
    texture_ = Game::assetManager->getTexture(texID, w, h);
    // updateRect_(w, h);
}


// void SpriteComponent::setTexture(const std::string& texID, int w, int h) {
//     texture_ = Game::assetManager->getTexture(texID);
//     updateRect_(w, h);
// }

// void SpriteComponent::play(const std::string& animName){
//     animFrames_ = animations_[animName].frames;
//     animIndex_ = animations_[animName].index;
//     animFPS_ = animations_[animName].fps;
// }

void SpriteComponent::updateRect_(int w, int h) {
    if (false) std::cout << w << " " << h; // to avoid unused parameter warning, to be removed when this function is implemented
    // transform_->w = w;
    // transform_->h = h;
    // srcRect_.w = w;
    // srcRect_.h = h;
    // dstRect_.w = static_cast<int>(transform_->scale * w);
    // dstRect_.h = static_cast<int>(transform_->scale * h);
}
