#include "ECS/SpriteComponent.hpp"
#include "TextureManager.hpp"

size_t SpriteComponent::instanceCount_ = 0;

// Constructors, destructor, and assignment operators
SpriteComponent::SpriteComponent() :
textureID(), layer(0), region(0), spriteFlip(SDL_FLIP_NONE), visible(true) {
    instanceCount_++;
}

SpriteComponent::SpriteComponent(const SpriteComponent& other) :
textureID(other.textureID), layer(other.layer), region(other.region), spriteFlip(other.spriteFlip), visible(other.visible) {
    instanceCount_++;
}

SpriteComponent::SpriteComponent(const TextureID& textureID) :
textureID(textureID), layer(0), region(0), spriteFlip(SDL_FLIP_NONE), visible(true) {
    instanceCount_++;
}

SpriteComponent::~SpriteComponent() {
    instanceCount_--;
}
