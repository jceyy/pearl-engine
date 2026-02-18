#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include "SDL2/SDL.h"
#include "Logging.hpp"
#include "TextureManager.hpp"
#include "ECS/Components.hpp"

size_t AssetManager::instanceCount_ = 0;

using namespace std;
using namespace PRL;

AssetManager::AssetManager(EntityManager* entityManager) : entityManager_(entityManager)
{
    assert(entityManager != nullptr);
    instanceCount_++;
}

AssetManager::~AssetManager() {
    instanceCount_--;
}

void AssetManager::addAssets(const std::string& assetID, const std::string& datFile) {
    assert(!assetID.empty());
    assert(!datFile.empty());

    std::ifstream in(datFile);
    if (!in) {
        Logging::err("Could not open asset data file : " + datFile, "PRL::AssetManager::addAssets");
        return;
    }
    Logging::log("Loading assets from file : " + datFile, "PRL::AssetManager::addAssets");

    std::string line;
    std::vector<std::string> lines;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        if (line[0] == '#') continue; // allow comments in .dat files starting with #
        lines.push_back(line.substr(line.find_first_not_of(" \t")));
    }

    // Look for sections
    std::vector<std::string> sections = {"[texture]", "[sprites]", "[animations]", "[tileset]"};
    std::map<std::string, std::vector<std::string>> sectionLines;
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
        Logging::err("No section found in asset data file : " + datFile, "PRL::AssetManager::addAssets");
        return;
    }
    // Load different assets based on sections
    TextureAsset tempAsset;
    loadTextureSection_(datFile, sectionLines["[texture]"], tempAsset);
    loadSpriteSection_(datFile, sectionLines["[sprites]"], tempAsset);
    textures_.emplace(assetID, std::move(tempAsset));
   
    loadAnimationSection_(sectionLines["[animations]"]);
    loadTilesetSection_(sectionLines["[tileset]"]);
    Logging::log("Animation section loading not implemented yet", "PRL::AssetManager::addAssets");
    return;
}

void AssetManager::loadTextureSection_(const string& datFile, const vector<string>& lines, TextureAsset& tempAsset) {
    if (lines.empty()) return;

    std::string textureFile, discard;
    for (const auto& l : lines) {
        std::istringstream iss(l);
        // Properties are native_resolution, and filtering mode
        if (l.find("native_resolution") == 0) {
            if (!(iss >> discard >> tempAsset.nativeResolution.x >> tempAsset.nativeResolution.y)) {
                Logging::err("Invalid native resolution format in asset data file " + datFile, "PRL::AssetManager::loadTextureSection");
                continue;
            }
        } else if (l.find("filtering_mode") == 0) {
            std::string mode;
            if (!(iss >> discard >> mode)) {
                Logging::err("Invalid filtering mode format in asset data file " + datFile, "PRL::AssetManager::loadTextureSection");
                continue;
            }
            if (mode == "nearest") tempAsset.scaleMode = SDL_ScaleModeNearest;
            else if (mode == "linear") tempAsset.scaleMode = SDL_ScaleModeLinear;
            else if (mode == "best" || mode == "anisotropic") tempAsset.scaleMode = SDL_ScaleModeBest;
            else {
                Logging::err("Unknown filtering mode '" + mode + "' in asset data file " + datFile, "PRL::AssetManager::loadTextureSection");
                continue;
            }
        } else if (l.find("file") == 0) {
            if (!(iss >> discard >> textureFile)) {
                Logging::err("Invalid texture file format in asset data file " + datFile, "PRL::AssetManager::loadTextureSection");
                continue;
            }
        }
    }

    // All section lines parsed
    tempAsset.texture = TextureManager::loadTexture(textureFile, tempAsset.textureSize.x, tempAsset.textureSize.y);
    return;
}

void AssetManager::loadSpriteSection_(const string& datFile, const vector<string>& lines, TextureAsset& tempAsset) {
    if (lines.empty()) return;

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
                Logging::err("Invalid default size format in asset data file " + datFile, "PRL::AssetManager::loadSpriteSection");
                continue;
            } else {
                defaultSizeSet = true;
            }
        } else if (l.find("default_pivot") == 0) {
            if (!(iss >> discard >> pxdef >> pydef)) {
                Logging::err("Invalid default pivot format in asset data file " + datFile, "PRL::AssetManager::loadSpriteSection");
                continue;
            } else {
                defaultPivotSet = true;
            }
        }
        else {
            // if (!(iss >> sprite_name >> x >> y >> w >> h >> discard >> sx >> sy >> discard >> px >> py)) {
            if (!(iss >> sprite_name >> x >> y >> w >> h)) {
                Logging::err("Invalid sprite region format in asset data file " + datFile, "PRL::AssetManager::loadSpriteSection");
                continue;
            } else if (iss >> discard) { // optional size and pivot data
                if (discard == "size") {
                    if (!(iss >> sx >> sy)) {
                        Logging::err("Invalid sprite size format in asset data file " + datFile, "PRL::AssetManager::loadSpriteSection");
                        continue;
                    }
                } else {
                    if (!defaultSizeSet) {
                        Logging::err("Sprite size not specified for sprite '" + sprite_name + "' in asset data file " + datFile + " and no default size set", 
                            "PRL::AssetManager::loadSpriteSection");
                        continue;
                    }
                    sx = sxdef;
                    sy = sydef;
                }

                if (discard == "pivot") {
                    if (!(iss >> px >> py)) {
                        Logging::err("Invalid sprite pivot format in asset data file " + datFile, "PRL::AssetManager::loadSpriteSection");
                        continue;
                    }
                } else {
                    if (!defaultPivotSet) {
                        Logging::err("Sprite pivot not specified for sprite '" + sprite_name + "' in asset data file " + datFile + " and no default pivot set", 
                            "PRL::AssetManager::loadSpriteSection");
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

void AssetManager::loadAnimationSection_(const std::vector<std::string>& lines) {
    if (lines.empty()) return;
}

void AssetManager::loadTilesetSection_(const std::vector<std::string>& lines) {
    if (lines.empty()) return;
}

void AssetManager::addTexture(const TextureID& id, const std::string& fileName) {
    if (false) {
        std::cout << id << " : " << fileName << std::endl;
    }
    // int wTexture(0), hTexture(0);

    // SDL_Texture* texture = TextureManager::loadTexture(fileName, wTexture, hTexture);
    // std::vector<SpriteRegion> regions = loadSpriteRegions_(fileName);
    // Vector2D nativeSize = Vector2D(wTexture, hTexture);

    // textures_.emplace(
    //     std::piecewise_construct,
    //     std::forward_as_tuple(id),
    //     std::forward_as_tuple(
    //         texture,
    //         std::move(regions),
    //         std::move(nativeSize)
    //     )
    // );
}

std::vector<SpriteRegion> AssetManager::loadSpriteRegions_(const std::string& fileName) {
    std::vector<SpriteRegion> regions;
    std::string datFile = fileName.substr(0, fileName.find_last_of('.')) + ".dat";
    std::ifstream in(datFile);
    if (!in) {
        // Temporary : will be an error
        Logging::log("Could not open sprite region data file : " + datFile, "PRL::AssetManager::loadSpriteRegions_");
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
            Logging::err("Invalid sprite region data format in file : " + datFile, "PRL::AssetManager::loadSpriteRegions_");
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