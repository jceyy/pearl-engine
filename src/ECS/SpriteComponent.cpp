#include "ECS/SpriteComponent.hpp"
#include "TextureManager.hpp"

SpriteComponent::SpriteComponent() :
transform_(nullptr), textureID_(""), texture_(nullptr), 
srcRect_({0, 0, 0, 0}), dstRect_({0, 0, 0, 0}), spriteFlip_(SDL_FLIP_NONE), 
animated_(false), animFrames_(0), animFPS_(0), animIndex_(0)  {
}


SpriteComponent::SpriteComponent(const std::string& textureID, bool animated) :
transform_(nullptr), textureID_(textureID), texture_(nullptr), 
srcRect_({0, 0, 0, 0}), dstRect_({0, 0, 0, 0}), spriteFlip_(SDL_FLIP_NONE), 
animated_(animated), animFrames_(0), animFPS_(0), animIndex_(0){
    Animation idle = Animation(0, 3, 6);
    Animation walk = Animation(1, 8, 16);
    animations_.emplace("Idle", idle);
    animations_.emplace("Walk", walk);
    play("Idle");  // By default
}


SpriteComponent::~SpriteComponent() {
}

void SpriteComponent::init() {
    entity->addComponent<TransformComponent>();
    transform_ = &entity->getComponent<TransformComponent>();

    srcRect_.x = srcRect_.y = 0;
    srcRect_.w = srcRect_.h = 0;
    dstRect_.x = dstRect_.y = 0.0f;
    dstRect_.w = dstRect_.h = 64;

if (textureID_ != std::string("")) {
        setTexture(textureID_);
    }
}

void SpriteComponent::update() {
    if (animated_) {
        srcRect_.x = srcRect_.w * static_cast<int>((SDL_GetTicks() * animFPS_ / 1000)
        % animFrames_);
    }
    srcRect_.y = animIndex_ * transform_->h;

    dstRect_.x = transform_->position.x - Game::camera.x;
    dstRect_.y = transform_->position.y - Game::camera.y;
    dstRect_.w = static_cast<int>(transform_->w * transform_->scale);
    dstRect_.h = static_cast<int>(transform_->h * transform_->scale);
}

void SpriteComponent::draw() {
    TextureManager::Draw(texture_, &srcRect_, &dstRect_, spriteFlip_);
}

void SpriteComponent::setTexture(const std::string& texID) {
    int w(0), h(0);
    texture_ = Game::assetManager->getTexture(texID, w, h);
    updateRect_(w, h);
}


void SpriteComponent::setTexture(const std::string& texID, int w, int h) {
    texture_ = Game::assetManager->getTexture(texID);
    updateRect_(w, h);
}

void SpriteComponent::play(const std::string& animName){
    animFrames_ = animations_[animName].frames;
    animIndex_ = animations_[animName].index;
    animFPS_ = animations_[animName].fps;
}

void SpriteComponent::setFlip(SDL_RendererFlip flip){
    spriteFlip_ = flip;
}

SDL_RendererFlip SpriteComponent::getFlip() const{
    return spriteFlip_;
}



void SpriteComponent::updateRect_(int w, int h) {
    transform_->w = w;
    transform_->h = h;
    srcRect_.w = w;
    srcRect_.h = h;
    dstRect_.w = static_cast<int>(transform_->scale * w);
    dstRect_.h = static_cast<int>(transform_->scale * h);
}
