#ifndef _ASSET_MANAGER_HPP_INCLUDED
#define _ASSET_MANAGER_HPP_INCLUDED

#include <unordered_map>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Types.hpp"
#include "ECS/ECS.hpp"
#include "Logging.hpp"


#if (SDL_MAJOR_VERSION == 2 && SDL_MINOR_VERSION == 0 && SDL_PATCHLEVEL < 12)
typedef enum SDL_ScaleMode {
    SDL_ScaleModeNearest = 0,
    SDL_ScaleModeLinear = 1,
    SDL_ScaleModeBest = 2
} SDL_ScaleMode;
#endif


template<typename AssetType>
struct Handle {
    std::size_t index = 0;
    std::size_t generation = 0;

    bool inline operator==(const Handle& other) const {
        return index == other.index && generation == other.generation;
    }

    bool inline operator!=(const Handle& other) const {
        return index != other.index || generation != other.generation;
    }
};

template<typename AssetType>
struct AssetSlot {
    AssetType asset;
    std::size_t generation = 0;
    bool assetInUse = false;
};


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
    TextureAsset() : texture(nullptr), nativeResolution(0, 0), textureSize(0, 0), scaleMode(SDL_ScaleModeNearest) {}
    TextureAsset(SDL_Texture* texture, const std::vector<SpriteRegion>& regions, const Vec2D<int>& nativeResolution, 
        const Vec2D<int> textureSize, SDL_ScaleMode scaleMode = SDL_ScaleModeNearest) :
        texture(texture), regions(regions), nativeResolution(nativeResolution), textureSize(textureSize), scaleMode(scaleMode) {}
    TextureAsset(const TextureAsset&) = delete;
    TextureAsset(TextureAsset&& other) noexcept : 
    texture(other.texture), regions(std::move(other.regions)), nativeResolution(other.nativeResolution), textureSize(other.textureSize), scaleMode(other.scaleMode) {
        other.texture = nullptr; // prevent destructor from freeing the texture
    }
    TextureAsset& operator=(const TextureAsset&) = delete;
    TextureAsset& operator=(TextureAsset&& other) noexcept {
        if (this != &other) {
            if (texture) SDL_DestroyTexture(texture); // free existing texture
            texture = other.texture;
            regions = std::move(other.regions);
            nativeResolution = other.nativeResolution;
            textureSize = other.textureSize;
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
    Vec2D<int> nativeResolution;    // internal design resolution
    Vec2D<int> textureSize;
    SDL_ScaleMode scaleMode;
};


class AnimationAsset {
public:
    AnimationAsset() : textureID(0), frameRegions(), fps(DEFAULT_ANIM_FPS), looping(true) {}
    AnimationAsset(std::size_t textureID, const std::vector<std::size_t>& frameRegions, float fps, bool looping) :
        textureID(textureID), frameRegions(frameRegions), fps(fps), looping(looping) {}
    AnimationAsset(const AnimationAsset&) = default;
    AnimationAsset(AnimationAsset&&) = default;
    AnimationAsset& operator=(const AnimationAsset&) = default;
    AnimationAsset& operator=(AnimationAsset&&) = default;

    std::size_t textureID;
    std::vector<std::size_t> frameRegions;  // region indices
    float fps;              // frames per second
    bool looping;
};

using TextureHandle = Handle<TextureAsset>;
using AnimationHandle = Handle<AnimationAsset>;


class AssetManager : public PRLObject {
public:
    AssetManager() = delete;
    AssetManager(EntityManager& entityManager);
    AssetManager(const AssetManager&) = delete;
    AssetManager(AssetManager&&) noexcept = default;
    ~AssetManager();
    AssetManager& operator=(const AssetManager&) = delete;
    AssetManager& operator=(AssetManager&&) noexcept = default;
    
    void addAsset(const std::string& assetID, const std::string& datFile);

    // Fonts
    void addFont(const std::string& fontID, const std::string& fileName, int fontSize);
    TTF_Font* getFont(const std::string& fontID);
    
    // Textures
    const inline TextureHandle getTextureHandle(const std::string& name) { 
        auto it = textureAssetHandles_.find(name);
        if (it == textureAssetHandles_.end()) {
            PRL::Logging::err("Texture asset '" + name + "' not found", "AssetManager::getTextureHandle");
        }
        assert(it != textureAssetHandles_.end());
        return it->second;
    }

    const inline TextureAsset* getTextureAsset(const TextureHandle& handle) noexcept {
        if (handle.index >= textureAssetSlots_.size())
            return nullptr;

        if (textureAssetSlots_[handle.index].generation != handle.generation)
            return nullptr;

        return &textureAssetSlots_[handle.index].asset;
    }
    
    // Animations
    const inline AnimationHandle getAnimationHandle(const std::string& name) {
        auto it = animationAssetHandles_.find(name);
        if (it == animationAssetHandles_.end()) {
            PRL::Logging::err("Animation asset '" + name + "' not found", "AssetManager::getAnimationHandle");
        }
        assert(it != animationAssetHandles_.end());
        return it->second;
    }

    const inline AnimationAsset* getAnimationAsset(const AnimationHandle& handle) noexcept {
        if (handle.index >= animationAssetSlots_.size())
            return nullptr;

        if (animationAssetSlots_[handle.index].generation != handle.generation)
            return nullptr;

        return &animationAssetSlots_[handle.index].asset;
    }
    
    // Game objects
    void createProjectile(Vector2D pos, Vector2D vel, float range, float speed, const TextureID& textureID); // to update with textureHandle
    
    static inline size_t getInstanceCount() noexcept { return instanceCount_; }

private:
    static void loadTextureSection_(const std::string& datFile, const std::vector<std::string>& lines, TextureAsset& tempAsset);
    static void loadSpriteSection_(const std::string& datFile, const std::vector<std::string>& lines, TextureAsset& tempAsset, std::unordered_map<std::string, std::size_t>& regionNames);
    static void loadAnimationSection_(const std::string& datFile, const std::vector<std::string>& lines, std::size_t textureIndex, std::vector<AnimationAsset>& tempAssets, std::vector<std::string>& animNames, const std::unordered_map<std::string, std::size_t>& regionNames);
    static void loadTilesetSection_(const std::vector<std::string>& lines);

    EntityManager& entityManager_;
    std::vector<AssetSlot<TextureAsset>> textureAssetSlots_;
    std::vector<std::size_t> textureAssetFreeList_;
    std::unordered_map<std::string, TextureHandle> textureAssetHandles_;

    std::vector<AssetSlot<AnimationAsset>> animationAssetSlots_;
    std::vector<std::size_t> animationAssetFreeList_;
    std::unordered_map<std::string, AnimationHandle> animationAssetHandles_;

    std::unordered_map<std::string, std::string> assetFilenameLookup_; // for debugging purposes, maps asset IDs to their source .dat files

    std::unordered_map<std::string, TTF_Font*> fonts_;

    static size_t instanceCount_;
};

#endif // _ASSET_MANAGER_HPP_INCLUDED