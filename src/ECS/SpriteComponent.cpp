#include "ECS/SpriteComponent.hpp"
#include "TextureManager.hpp"

size_t SpriteComponent::count_ = 0;

// Constructors, destructor, and assignment operators
SpriteComponent::SpriteComponent() :
textureID_(""), texture_(nullptr), 
srcRect_({0, 0, 0, 0}), dstRect_({0, 0, 0, 0}), spriteFlip_(SDL_FLIP_NONE) {
    count_++;
}

SpriteComponent::SpriteComponent(const SpriteComponent& other) :
textureID_(other.textureID_), texture_(other.texture_), 
srcRect_(other.srcRect_), dstRect_(other.dstRect_), spriteFlip_(other.spriteFlip_) {
    count_++;
}

SpriteComponent::SpriteComponent(SpriteComponent&& other) noexcept :
textureID_(std::move(other.textureID_)), texture_(other.texture_),
srcRect_(other.srcRect_), dstRect_(other.dstRect_), spriteFlip_(other.spriteFlip_) {
    other.texture_ = nullptr;
}

SpriteComponent::SpriteComponent(const std::string& textureID) :
textureID_(textureID), texture_(nullptr), 
srcRect_({0, 0, 0, 0}), dstRect_({0, 0, 0, 0}), spriteFlip_(SDL_FLIP_NONE) {
    count_++;
}

SpriteComponent::~SpriteComponent() {
    count_--;
}

SpriteComponent& SpriteComponent::operator=(const SpriteComponent& other) {
    if (this != &other) {
        textureID_ = other.textureID_;
        texture_ = other.texture_;
        srcRect_ = other.srcRect_;
        dstRect_ = other.dstRect_;
        spriteFlip_ = other.spriteFlip_;
    }
    return *this;
}

SpriteComponent& SpriteComponent::operator=(SpriteComponent&& other) noexcept {
    if (this != &other) {
        textureID_ = std::move(other.textureID_);
        texture_ = other.texture_;
        srcRect_ = other.srcRect_;
        dstRect_ = other.dstRect_;
        spriteFlip_ = other.spriteFlip_;
        other.texture_ = nullptr;
    }
    return *this;
}


void SpriteComponent::init() {
    if (textureID_ != std::string("")) {
        setTexture(textureID_);
    }
}

void SpriteComponent::update() {
    // if (animated_) {
    //     srcRect_.x = srcRect_.w * static_cast<int>((SDL_GetTicks() * animFPS_ / 1000)
    //     % animFrames_);
    // }
    // srcRect_.y = animIndex_ * transform_->h;

    // dstRect_.x = transform_->position.x - Game::camera.x;
    // dstRect_.y = transform_->position.y - Game::camera.y;
    // dstRect_.w = static_cast<int>(transform_->w * transform_->scale);
    // dstRect_.h = static_cast<int>(transform_->h * transform_->scale);
}

void SpriteComponent::draw() {
    // TextureManager::Draw(texture_, &srcRect_, &dstRect_, spriteFlip_);
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

void SpriteComponent::setFlip(SDL_RendererFlip flip){
    spriteFlip_ = flip;
}

SDL_RendererFlip SpriteComponent::getFlip() const noexcept {
    return spriteFlip_;
}



void SpriteComponent::updateRect_(int w, int h) {
    if (false) std::cout << w << " " << h; // to avoid unused parameter warning, to be removed when this function is implemented
    // transform_->w = w;
    // transform_->h = h;
    // srcRect_.w = w;
    // srcRect_.h = h;
    // dstRect_.w = static_cast<int>(transform_->scale * w);
    // dstRect_.h = static_cast<int>(transform_->scale * h);
}
