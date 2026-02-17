#ifndef _ASSET_MANAGER_HPP_INCLUDED
#define _ASSET_MANAGER_HPP_INCLUDED

#include <map>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Types.hpp"
#include "ECS/ECS.hpp"


class SpriteRegion : public SDL_Rect {
public:
    SpriteRegion() : SDL_Rect{0, 0, 0, 0} {
        // x = y = w = h = 0;
    }
    SpriteRegion(int x, int y, int w, int h) : SDL_Rect{x, y, w, h} {}
    
    // int x, y, w, h;   // in texture pixels
};


class TextureAsset {
public:
    TextureAsset() : texture(nullptr), nativeSpriteSize(0, 0) {}
    TextureAsset(SDL_Texture* texture, const std::vector<SpriteRegion>& regions, const Vector2D& nativeSpriteSize) :
    texture(texture), regions(regions), nativeSpriteSize(nativeSpriteSize) {}
    TextureAsset(const TextureAsset&) = delete;
    TextureAsset(TextureAsset&& other) noexcept : 
    texture(other.texture), regions(std::move(other.regions)), nativeSpriteSize(other.nativeSpriteSize) {
        other.texture = nullptr; // prevent destructor from freeing the texture
    }
    TextureAsset& operator=(const TextureAsset&) = delete;
    TextureAsset& operator=(TextureAsset&& other) noexcept {
        if (this != &other) {
            if (texture) SDL_DestroyTexture(texture); // free existing texture
            texture = other.texture;
            regions = std::move(other.regions);
            nativeSpriteSize = other.nativeSpriteSize;
            other.texture = nullptr; // prevent destructor from freeing the texture
        }
        return *this;
    }

    ~TextureAsset() {
        if (texture) SDL_DestroyTexture(texture);
    }

    SDL_Texture* texture;
    std::vector<SpriteRegion> regions;
    Vector2D nativeSpriteSize;    // internal design resolution
};


class AnimationClip {
public:
    AnimationClip() : frames(0), fps(DEFAULT_ANIM_FPS), looping(true) {}

    std::vector<std::size_t> frames;  // region indices
    float fps;              // frames per second
    bool looping;
};


class AssetManager : public PRLObject {
public:
    AssetManager() = delete;
    AssetManager(EntityManager* entityManager);
    AssetManager(const AssetManager&) = delete;
    AssetManager(AssetManager&&) noexcept = default;
    ~AssetManager();
    AssetManager& operator=(const AssetManager&) = delete;
    AssetManager& operator=(AssetManager&&) noexcept = default;
    
    // Textures
    void addTexture(const TextureID& id, const std::string& fileName);
    const inline TextureAsset& getTexture(const TextureID& id) { return textures_.at(id); }

    // Animations
    void addAnimation(const AnimationID& id, const AnimationClip& clip);
    const inline AnimationClip& getAnimation(const AnimationID& id) const { return animations_.at(id); }
    
    // Fonts
    void addFont(const std::string& fontID, const std::string& fileName, int fontSize);
    TTF_Font* getFont(const std::string& fontID);
    
    // Game objects
    void createProjectile(Vector2D pos, Vector2D vel, float range, float speed, const TextureID& textureID);
    
    static inline size_t getInstanceCount() noexcept { return instanceCount_; }

private:
    static std::vector<SpriteRegion> loadSpriteRegions_(const std::string& fileName);

    EntityManager* entityManager_;
    // std::map<std::string, SDL_Texture*> textures_;  // Add string_paths to be able to reload
    // std::map<std::string, std::pair<int, int>> texturesSize_;
    std::map<TextureID, TextureAsset> textures_;
    std::map<AnimationID, AnimationClip> animations_;
    std::map<std::string, TTF_Font*> fonts_;

    static size_t instanceCount_;
};

#endif // _ASSET_MANAGER_HPP_INCLUDED