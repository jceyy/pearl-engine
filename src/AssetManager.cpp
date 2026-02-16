#include "TextureManager.hpp"
#include "ECS/Components.hpp"

size_t AssetManager::instanceCount_ = 0;

AssetManager::AssetManager(EntityManager* entityManager) : entityManager_(entityManager)
{
    assert(entityManager != nullptr);
    instanceCount_++;
}

AssetManager::~AssetManager() {
    instanceCount_--;
}

void AssetManager::addTexture(const TextureID& id, const std::string& fileName) {
    int wTexture(0), hTexture(0);
    TextureAsset textureAsset(
        TextureManager::loadTexture(fileName, wTexture, hTexture), 
        {SpriteRegion(0, 0, wTexture, hTexture)}, 
        Vector2D(wTexture, hTexture)
    );
    textures_.emplace(id, textureAsset);
}

void AssetManager::addAnimation(const AnimationID& id, const AnimationClip& clip) {
    animations_.emplace(id, clip);
}

void AssetManager::createProjectile(Vector2D pos, Vector2D vel, float range, float speed, const TextureID& textureID){
    auto& projectile(entityManager_->addEntity());
    projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
    projectile.addComponent<SpriteComponent>(textureID);
    projectile.addComponent<ProjectileComponent>(range, speed, vel);
    // projectile.addComponent<ColliderComponent>("projectile");
    projectile.addGroup(Game::groupProjectiles); 
}

void AssetManager::addFont(const std::string& fontID, const std::string& fileName, int fontSize){
    TTF_Font* font = TTF_OpenFont(fileName.c_str(), fontSize);
    fonts_.emplace(fontID, font);
}


TTF_Font* AssetManager::getFont(const std::string& fontID){
    return fonts_[fontID];
}