#include "AssetManager.hpp"
#include "TextureManager.hpp"
#include "ECS/Components.hpp"

AssetManager::AssetManager(EntityManager* entityManager) : entityManager_(entityManager)
{}

AssetManager::~AssetManager() {}

void AssetManager::addTexture(const std::string& id, const std::string& fileName) {
    int w(0), h(0);
    textures_.emplace(id, TextureManager::loadTexture(fileName, w, h));
    texturesSize_.emplace(id, std::pair<int,int>(w, h));
}


void AssetManager::createProjectile(Vector2D pos, Vector2D vel, float range, float speed, const std::string& textureID){
    auto& projectile(entityManager_->addEntity());
    projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
    projectile.addComponent<SpriteComponent>(textureID, false);
    projectile.addComponent<ProjectileComponent>(range, speed, vel);
    projectile.addComponent<ColliderComponent>("projectile");
    projectile.addGroup(Game::groupProjectiles); 
}


SDL_Texture* AssetManager::getTexture(const std::string& id){
    return textures_[id];
}

SDL_Texture* AssetManager::getTexture(const std::string& id, int& w, int& h){
    w = texturesSize_[id].first;
    h = texturesSize_[id].second;
    return textures_[id];
}


void AssetManager::addFont(const std::string& fontID, const std::string& fileName, int fontSize){
    TTF_Font* font = TTF_OpenFont(fileName.c_str(), fontSize);
    fonts_.emplace(fontID, font);
}


TTF_Font* AssetManager::getFont(const std::string& fontID){
    return fonts_[fontID];
}