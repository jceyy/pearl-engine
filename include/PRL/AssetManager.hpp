#ifndef _ASSET_MANAGER_HPP_INCLUDED
#define _ASSET_MANAGER_HPP_INCLUDED


#include <map>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Types.hpp"
#include "ECS/ECS.hpp"


class AssetManager {
public:
    AssetManager(EntityManager* entityManager);
    ~AssetManager();

    // Game objects
    void createProjectile(Vector2D pos, Vector2D vel, float range, float speed, const std::string& textureID);

    // Textures
    void addTexture(const std::string& id, const std::string& fileName);
    SDL_Texture* getTexture(const std::string& id);
    SDL_Texture* getTexture(const std::string& id, int& w, int& h);

    // Fonts
    void addFont(const std::string& fontID, const std::string& fileName, int fontSize);
    TTF_Font* getFont(const std::string& fontID);

private:
    EntityManager* entityManager_;
    std::map<std::string, SDL_Texture*> textures_;  // Add string_paths to be able to reload
    std::map<std::string, std::pair<int, int>> texturesSize_;
    std::map<std::string, TTF_Font*> fonts_;
};

#endif // _ASSET_MANAGER_HPP_INCLUDED