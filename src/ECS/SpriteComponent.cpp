#include "Core.hpp"
#include "ECS/SpriteComponent.hpp"

namespace PRL {

size_t SpriteComponent::instanceCount_ = 0;

// Constructors, destructor, and assignment operators
SpriteComponent::SpriteComponent() :
textureHandle(), layer(0), region(0), spriteFlip(SDL_FLIP_NONE), visible(true) {
    instanceCount_++;
}

SpriteComponent::SpriteComponent(const SpriteComponent& other) :
textureHandle(other.textureHandle), layer(other.layer), region(other.region), spriteFlip(other.spriteFlip), visible(other.visible) {
    instanceCount_++;
}

SpriteComponent::SpriteComponent(const std::string& textureName, const std::string& regionName) :
textureHandle(PRL::Core::getAssetManager().getTextureHandle(textureName)), layer(0), region(0), spriteFlip(SDL_FLIP_NONE), visible(true) {
    instanceCount_++;
    if (!regionName.empty()) {
        const TextureAsset* textureAsset = PRL::Core::getAssetManager().getTextureAsset(textureHandle);
        auto it = textureAsset->regionNames.find(regionName);

        if (it != textureAsset->regionNames.end()) {
            region = it->second;
        } else {
            PRL::Logging::err("Region name '" + regionName + "' not found in texture asset when creating SpriteComponent with texture '" 
                + textureName + "'", "PRL::SpriteComponent constructor");
        }
    }
}

SpriteComponent::~SpriteComponent() {
    instanceCount_--;
}

} // namespace PRL
