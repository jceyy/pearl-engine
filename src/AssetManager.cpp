#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include "SDL2/SDL.h"
#include "AssetManager.hpp"
#include "Logging.hpp"
#include "TextureManager.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"

using namespace std;

namespace PRL {

size_t AssetManager::instanceCount_ = 0;

AssetManager::AssetManager(EntityManager& entityManager) : entityManager_(entityManager)
{
    instanceCount_++;
}

AssetManager::~AssetManager() {
    instanceCount_--;
}

void AssetManager::addAsset(const std::string& assetID, const std::string& datFile) {
    // Non-empty strings for assetID and datFile path
    if (assetID.empty() || datFile.empty()) {
        Logging::err("Asset ID and data file path cannot be empty.", "PRL::AssetManager::addAsset()");
        return;
    }

    // Lookup to prevent duplicate asset IDs
    if (textureAssetHandles_.find(assetID) != textureAssetHandles_.end()) {
        Logging::err("In loading '" + datFile + "', asset ID '" + assetID + "' already exists.", "PRL::AssetManager::addAsset()");
        return;
    }

    std::ifstream in(datFile);
    if (!in) {
        Logging::err("Could not open asset data file : " + datFile, "PRL::AssetManager::addAsset()");
        return;
    }
    Logging::log("Loading asset from file : " + datFile, "PRL::AssetManager::addAsset()");
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        if (line[0] == '#') continue; // allow comments in .dat files starting with #
        lines.push_back(line.substr(line.find_first_not_of(" \t")));
    }

    // Look for sections
    std::vector<std::string> sections = {"[texture]", "[sprites]", "[animations]"};
    std::unordered_map<std::string, std::vector<std::string>> sectionLines;
    std::string currentSection("");
    for (const auto& l : lines) {
        if (std::find(sections.begin(), sections.end(), l) != sections.end()) {
            currentSection = l;
        }
        else if (!currentSection.empty()) {
            sectionLines[currentSection].push_back(l);
        }
    }
    if (currentSection.empty()) {
        Logging::err("No section found in asset data file : " + datFile, "PRL::AssetManager::addTextureAsset()");
        return;
    }

    // Load different assets based on sections
    TextureAsset tempAsset;
    loadTextureSection_(datFile, sectionLines["[texture]"], tempAsset);
    loadSpriteSection_(datFile, sectionLines["[sprites]"], tempAsset);

    // Assign a new texture asset slot
    size_t textureIndex;
    if (!textureAssetFreeList_.empty()) {
        textureIndex = textureAssetFreeList_.back();
        textureAssetFreeList_.pop_back();
    } else {
        textureIndex = textureAssetSlots_.size();
        textureAssetSlots_.emplace_back();
    }

    textureAssetSlots_[textureIndex].asset = std::move(tempAsset);
    textureAssetSlots_[textureIndex].generation++;
    textureAssetSlots_[textureIndex].assetInUse = true;
    textureAssetHandles_.emplace(
        assetID, 
        TextureHandle{textureIndex, textureAssetSlots_[textureIndex].generation}
    );

    // Animations
    std::vector<AnimationAsset> tempAnimations;
    std::vector<std::string> animNames;
    loadAnimationSection_(datFile, sectionLines["[animations]"], textureIndex, tempAnimations, animNames, textureAssetSlots_[textureIndex].asset);
    for (size_t i(0); i < tempAnimations.size(); ++i) {
        // Assign a new animation asset slot
        size_t animIndex;
        if (!animationAssetFreeList_.empty()) {
            animIndex = animationAssetFreeList_.back();
            animationAssetFreeList_.pop_back();
        } else {
            animIndex = animationAssetSlots_.size();
            animationAssetSlots_.emplace_back();
        }
        animationAssetSlots_[animIndex].asset = std::move(tempAnimations[i]);
        animationAssetSlots_[animIndex].generation++;
        animationAssetSlots_[animIndex].assetInUse = true;

        animationAssetHandles_.emplace(
            assetID + "." + animNames[i],
            AnimationHandle{animIndex, animationAssetSlots_[animIndex].generation}
        );
    }
}

void AssetManager::loadTextureSection_(const string& datFile, const vector<string>& lines, TextureAsset& tempAsset) {
    if (lines.empty()) {
        PRL::Logging::err("No lines in [texture] section of asset data file : " + datFile, "PRL::AssetManager::loadTextureSection_()");
        return;
    }

    std::string textureFile, discard;
    for (const auto& l : lines) {
        std::istringstream iss(l);
        // Properties are native_resolution, and filtering mode
        if (l.find("native_resolution") == 0) {
            if (!(iss >> discard >> tempAsset.nativeResolution.x >> tempAsset.nativeResolution.y)) {
                Logging::err("Invalid native resolution format in asset data file " + datFile, "PRL::AssetManager::loadTextureSection_()");
                continue;
            }
        } else if (l.find("filtering_mode") == 0) {
            std::string mode;
            if (!(iss >> discard >> mode)) {
                Logging::err("Invalid filtering mode format in asset data file " + datFile, "PRL::AssetManager::loadTextureSection_()");
                continue;
            }
            if (mode == "nearest") tempAsset.scaleMode = SDL_ScaleModeNearest;
            else if (mode == "linear") tempAsset.scaleMode = SDL_ScaleModeLinear;
            else if (mode == "best" || mode == "anisotropic") tempAsset.scaleMode = SDL_ScaleModeBest;
            else {
                Logging::err("Unknown filtering mode '" + mode + "' in asset data file " + datFile, "PRL::AssetManager::loadTextureSection_()");
                continue;
            }
        } else if (l.find("file") == 0) {
            if (!(iss >> discard >> textureFile)) {
                Logging::err("Invalid texture file format in asset data file " + datFile, "PRL::AssetManager::loadTextureSection_()");
                continue;
            }
        }
    }

    // All section lines parsed
    tempAsset.texture = TextureManager::loadTexture(textureFile, tempAsset.textureSize.x, tempAsset.textureSize.y);
    #if SDL_VERSION_ATLEAST(3,2,0)
    if (tempAsset.texture) {
        if (!SDL_SetTextureScaleMode(tempAsset.texture, tempAsset.scaleMode)) {
            Logging::err("Failed to set texture scale mode for texture '" + textureFile + "' in asset data file " + datFile + " : " + SDL_GetError(), "PRL::AssetManager::loadTextureSection_()");
        }
    } else {
        Logging::err("Failed to load texture '" + textureFile + "' in asset data file " + datFile + " : " + SDL_GetError(), "PRL::AssetManager::loadTextureSection_()");
    }
    #endif // SDL3 -- not doing the change yet
    return;
}

void AssetManager::loadSpriteSection_(const string& datFile, const vector<string>& lines, TextureAsset& tempAsset) {
    if (lines.empty()) {
        PRL::Logging::err("No lines in [sprites] section of asset data file : " + datFile, "PRL::AssetManager::loadSpriteSection_()");
        return;
    }

    float sxdef(1.0f), sydef(1.0f), pxdef(0.0f), pydef(0.0f);
    bool defaultSizeSet(false), defaultPivotSet(false);
    int x, y, w, h;
    float sx, sy, px, py;
    size_t nSprites(0);
    std::string discard, sprite_name;
    for (const auto& l : lines) {
        std::istringstream iss(l);

        if (l.find("default_size") == 0) {
            if (!(iss >> discard >> sxdef >> sydef)) {
                Logging::err("Invalid default size format in asset data file " + datFile, "PRL::AssetManager::loadSpriteSection_()");
                continue;
            } else {
                defaultSizeSet = true;
            }
        } else if (l.find("default_pivot") == 0) {
            if (!(iss >> discard >> pxdef >> pydef)) {
                Logging::err("Invalid default pivot format in asset data file " + datFile, "PRL::AssetManager::loadSpriteSection_()");
                continue;
            } else {
                defaultPivotSet = true;
            }
        }
        else {
            // if (!(iss >> sprite_name >> x >> y >> w >> h >> discard >> sx >> sy >> discard >> px >> py)) {
            if (!(iss >> sprite_name >> x >> y >> w >> h)) {
                Logging::err("Invalid sprite region format in asset data file " + datFile, "PRL::AssetManager::loadSpriteSection_()");
                continue;
            } else if (iss >> discard) { // optional size and pivot data
                if (discard == "size") {
                    if (!(iss >> sx >> sy)) {
                        Logging::err("Invalid sprite size format in asset data file " + datFile, "PRL::AssetManager::loadSpriteSection_()");
                        continue;
                    }
                } else {
                    if (!defaultSizeSet) {
                        Logging::err("Sprite size not specified for sprite '" + sprite_name + "' in asset data file " + datFile + " and no default size set", 
                            "PRL::AssetManager::loadSpriteSection_()");
                        continue;
                    }
                    sx = sxdef;
                    sy = sydef;
                }

                if (discard == "pivot") {
                    if (!(iss >> px >> py)) {
                        Logging::err("Invalid sprite pivot format in asset data file " + datFile, "PRL::AssetManager::loadSpriteSection_()");
                        continue;
                    }
                } else {
                    if (!defaultPivotSet) {
                        Logging::err("Sprite pivot not specified for sprite '" + sprite_name + "' in asset data file " + datFile + " and no default pivot set", 
                            "PRL::AssetManager::loadSpriteSection_()");
                        continue;
                    }
                    px = pxdef;
                    py = pydef;
                }
            } else {
                sx = sxdef;
                sy = sydef;
                px = pxdef;
                py = pydef;

            }
            tempAsset.regions.emplace_back(x, y, w, h, sx, sy, px, py);
            tempAsset.regionNames.emplace(sprite_name, nSprites++);
            // Map automatically checks for duplicate keys, so this will ensure sprite names are unique
        }
    }
    return;
}

void AssetManager::loadAnimationSection_(const std::string& datFile, const std::vector<std::string>& lines, std::size_t textureIndex, std::vector<AnimationAsset>& tempAssets, 
    std::vector<std::string>& animNames, const TextureAsset& textureAsset) {
    if (lines.empty()) return;

    tempAssets.clear(); // should already be empty but just to be safe
    float fps(DEFAULT_ANIM_FPS);
    bool looping(false);
    std::string discard, animName, frame;
    for (const auto& l : lines) {
        std::istringstream iss(l);

        if (!(iss >> animName)) {
            PRL::Logging::err("Unable to read animation name in asset datafile " + datFile, "AssetManager::loadAnimationSection_()");
            continue;
        }
        if (!(iss >> discard >> fps >> discard >> looping)) {
            PRL::Logging::err("Unable to load fps or looping properties when reading animation " + animName + " in asset datafile " + datFile, "AssetManager::loadAnimationSection_()");
            continue;
        }

        // Now read frames until end of line
        std::vector<std::string> frameNames;
        if (!(iss >> discard) || discard != "frames") {
            PRL::Logging::err("Missing 'frames' keyword in animation data for animation " + animName + " in asset datafile " + datFile, "AssetManager::loadAnimationSection_()");
            continue;
        }

        while (iss >> frame) {
            frameNames.push_back(frame);
        }
        if (frameNames.empty()) {
            PRL::Logging::err("No frames specified for animation " + animName + " in asset datafile " + datFile, "AssetManager::loadAnimationSection_()");
            continue;
        }

        // Convert frame names to region indices
        std::vector<std::size_t> frameRegions;
        for (size_t i(0); i < frameNames.size(); ++i) {
            const std::string& regName = frameNames[i];
            auto it = textureAsset.regionNames.find(regName);
            if (it != textureAsset.regionNames.end()) {
                frameRegions.push_back(it->second);
            }
            if (it == textureAsset.regionNames.end()) {
                PRL::Logging::err("Frame '" + regName + "' not found in texture regions when reading animation " + animName + " in asset datafile " + datFile, "AssetManager::loadAnimationSection_()");
                continue;
            }
        }
        animNames.push_back(animName);

        // Construct new asset
        tempAssets.emplace_back(textureIndex, std::move(frameRegions), fps, looping);
    }
}

void AssetManager::createProjectile(Vector2D pos, Vector2D vel, float range, float speed, const TextureID& textureID){
    auto& projectile(entityManager_.addEntity());
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

} // namespace PRL