#include <iostream>
#include <fstream>
#include <sstream>
#include "SDL2/SDL.h"
#include "Logging.hpp"
#include "TextureManager.hpp"
#include "ECS/Components.hpp"

size_t AssetManager::instanceCount_ = 0;

using namespace PRL;

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

    SDL_Texture* texture = TextureManager::loadTexture(fileName, wTexture, hTexture);
    std::vector<SpriteRegion> regions = loadSpriteRegions_(fileName);
    Vector2D nativeSize = Vector2D(wTexture, hTexture);

    textures_.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(id),
        std::forward_as_tuple(
            texture,
            std::move(regions),
            std::move(nativeSize)
        )
    );
}

std::vector<SpriteRegion> AssetManager::loadSpriteRegions_(const std::string& fileName) {
    std::vector<SpriteRegion> regions;
    std::string datFile = fileName.substr(0, fileName.find_last_of('.')) + ".dat";
    std::ifstream in(datFile);
    if (!in) {
        // Temporary : will be an error
        Logging::log("Could not open sprite region data file : " + datFile, "AssetManager::loadSpriteRegions_");
        return regions; // return empty regions vector, the texture will still be loaded but with no regions
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        if (line[0] == '#') continue; // allow comments in .dat files starting with #

        std::istringstream iss(line);
        // Each line must contain Wsprite Hsprite Nsprites [fps loop]
        int w, h, n;

        if (!(iss >> w >> h >> n)) {
            Logging::err("Invalid sprite region data format in file : " + datFile, "AssetManager::loadSpriteRegions_");
            continue;
        }
        assert(w > 0 && h > 0 && n > 0);
        // for now ignore fps and loop data, that is for animation
        
        for (int i = 0; i < n; ++i) {
            regions.emplace_back(i * w, lineNumber * h, w, h);
        }
        lineNumber++;
    }
    return regions;
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