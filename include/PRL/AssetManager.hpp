#ifndef _ASSET_MANAGER_HPP_INCLUDED
#define _ASSET_MANAGER_HPP_INCLUDED

#include <map>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Types.hpp"
#include "ECS/ECS.hpp"


#if (SDL_MAJOR_VERSION == 2 && SDL_MINOR_VERSION == 0 && SDL_PATCHLEVEL < 12)
typedef enum SDL_ScaleMode {
    SDL_ScaleModeNearest = 0,
    SDL_ScaleModeLinear = 1,
    SDL_ScaleModeBest = 2
} SDL_ScaleMode;
#endif

class SpriteRegion {
public:
    SpriteRegion() : x(0), y(0), w(0), h(0) {}
    SpriteRegion(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
    SpriteRegion(int x, int y, int w, int h, PosType sx, PosType sy, PosType px, PosType py) : 
        x(x), y(y), w(w), h(h), size(sx, sy), pivot(px, py) {}
    
    int x, y, w, h;      // in texture pixels
    Vec2D<PosType> size;
    Vec2D<PosType> pivot;
};


class TextureAsset {
public:
    TextureAsset() : texture(nullptr), nativeResolution(0, 0), scaleMode(SDL_ScaleModeNearest) {}
    TextureAsset(SDL_Texture* texture, const std::vector<SpriteRegion>& regions, const Vec2D<int>& nativeResolution, SDL_ScaleMode scaleMode = SDL_ScaleModeNearest) :
        texture(texture), regions(regions), nativeResolution(nativeResolution), scaleMode(scaleMode) {}
    TextureAsset(const TextureAsset&) = delete;
    TextureAsset(TextureAsset&& other) noexcept : 
    texture(other.texture), regions(std::move(other.regions)), nativeResolution(other.nativeResolution), scaleMode(other.scaleMode) {
        other.texture = nullptr; // prevent destructor from freeing the texture
    }
    TextureAsset& operator=(const TextureAsset&) = delete;
    TextureAsset& operator=(TextureAsset&& other) noexcept {
        if (this != &other) {
            if (texture) SDL_DestroyTexture(texture); // free existing texture
            texture = other.texture;
            regions = std::move(other.regions);
            nativeResolution = other.nativeResolution;
            scaleMode = other.scaleMode;
            other.texture = nullptr; // prevent destructor from freeing the texture
        }
        return *this;
    }

    ~TextureAsset() {
        if (texture) SDL_DestroyTexture(texture);
    }

    SDL_Texture* texture;
    std::vector<SpriteRegion> regions;
    std::map<std::string, size_t> regionNames; // optional, for easier access to regions by name
    Vec2D<int> nativeResolution;    // internal design resolution
    Vec2D<int> textureSize;
    SDL_ScaleMode scaleMode;
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
    
    void addAssets(const std::string& assetID, const std::string& datFile);

    // Fonts
    void addFont(const std::string& fontID, const std::string& fileName, int fontSize);
    TTF_Font* getFont(const std::string& fontID);
    
    // Textures
    void addTexture(const TextureID& id, const std::string& fileName);
    const inline TextureAsset& getTexture(const TextureID& id) { return textures_.at(id); }

    // Animations
    void addAnimation(const AnimationID& id, const AnimationClip& clip);
    const inline AnimationClip& getAnimation(const AnimationID& id) const { return animations_.at(id); }
    
    // Game objects
    void createProjectile(Vector2D pos, Vector2D vel, float range, float speed, const TextureID& textureID);
    
    static inline size_t getInstanceCount() noexcept { return instanceCount_; }

private:
    static std::vector<SpriteRegion> loadSpriteRegions_(const std::string& fileName);
    static void loadTextureSection_(const std::string& datFile, const std::vector<std::string>& lines, TextureAsset& tempAsset);
    static void loadSpriteSection_(const std::string& datFile, const std::vector<std::string>& lines, TextureAsset& tempAsset);
    static void loadAnimationSection_(const std::vector<std::string>& lines);
    static void loadTilesetSection_(const std::vector<std::string>& lines);


    EntityManager* entityManager_;
    // std::map<std::string, SDL_Texture*> textures_;  // Add string_paths to be able to reload
    // std::map<std::string, std::pair<int, int>> texturesSize_;
    std::map<TextureID, TextureAsset> textures_;
    std::map<AnimationID, AnimationClip> animations_;
    std::map<std::string, TTF_Font*> fonts_;

    static size_t instanceCount_;
};

#endif // _ASSET_MANAGER_HPP_INCLUDED