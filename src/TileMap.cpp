#include <fstream>
#include <sstream>
#include <cassert>
#include "Core.hpp"
#include "Types.hpp"
#include "TileMap.hpp"
#include "Logging.hpp"

namespace PRL {

size_t TileMap::instanceCount_ = 0;

TileMap::TileMap() : tileSize(0, 0), mapSize(0, 0), chunkCount(0, 0), layers(), chunks() {
        instanceCount_++;
}

TileMap::~TileMap() {
    instanceCount_--;
}

void TileMap::loadMap(const std::string& fileName) {
    if (layers.size() > 0)
        layers.clear();
    if (chunks.size() > 0)
        chunks.clear();

    std::ifstream mapStream(fileName);
    if (!mapStream) {
        Logging::err("Could not open map file: " + fileName, "PRL::TileMap::loadMap");
        return;
    }
    Logging::log("Loading asset from file : " + fileName, "PRL::TileMap::loadMap");
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(mapStream, line)) {
        if (line.empty()) continue;
        if (line[0] == '#') continue; // allow comments in .dat files starting with #
        lines.push_back(line.substr(line.find_first_not_of(" \t")));
    }

    // Multiple sections can be layer-focused
    std::vector<std::string> sections = {"[properties]", "[table]", "[collision]"};
    std::unordered_map<std::string, std::vector<std::string>> sectionLines;
    std::vector<std::vector<std::string>> layerSections; // to keep track of layer sections in order

    std::string currentSection("");
    bool readingLayerSection = false;

    for (const auto& l : lines) {
        if (std::find(sections.begin(), sections.end(), l) != sections.end()) {
            currentSection = l;
            readingLayerSection = false;
        }
        else if (l == "[layer]") {
            readingLayerSection = true;
            layerSections.emplace_back(); // start a new layer section
        }
        else if (!currentSection.empty()) {
            if (readingLayerSection) {
                layerSections.back().push_back(l);
            }
            else {
                sectionLines[currentSection].push_back(l);
            }
        }
    }

    if (currentSection.empty()) {
        Logging::err("No section found in tilemap data file : " + fileName, "PRL::TileMap::loadMap");
        return;
    }
    if (layerSections.empty()) {
        Logging::err("No layer sections found in tilemap data file : " + fileName, "PRL::TileMap::loadMap");
        return;
    }

    // Now load data from sections
    // AssetManager& assetManager = PRL::Core::getAssetManager();
    std::vector<std::string> renderOrder;
    std::vector<std::vector<Tile>> fullTilemap;
    std::unordered_map<int, std::tuple<TextureHandle, AnimationHandle, bool>> tileTable;
    
    // Load all sections in the file
    return;
    loadPropertiesSection_(sectionLines["[properties]"], renderOrder);
    loadTableSection_(sectionLines["[table]"], tileTable);
    loadLayerSections_(layerSections, renderOrder, tileTable, fullTilemap);
    loadCollisionSection_(sectionLines["[collision]"]);
    

    return;
}


void TileMap::loadPropertiesSection_(const std::vector<std::string>& lines, std::vector<std::string>& renderOrder) {
    if (lines.empty()) {
        PRL::Logging::err("No lines in [properties] section of tilemap data file", "TileMap::loadPropertiesSection_");
        return;
    }
    if (renderOrder.size() > 0) {
        renderOrder.clear();
    }

    std::string discard, layerName;
    mapSize = Vec2D<int>(0, 0);
    for (const auto& l : lines) {
        std::istringstream iss(l);
        if (l.find("map_size") == 0) {
            if (!(iss >> discard >> mapSize.x >> mapSize.y)) {
                Logging::err("Invalid map size format in tilemap data file", "TileMap::loadPropertiesSection_");
                continue;
            }
        } else if (l.find("render_order") == 0) {
            if (!(iss >> discard >> layerName)) {
                Logging::err("Invalid render order format in tilemap data file", "TileMap::loadPropertiesSection_");
                continue;
            }
            else {
                renderOrder.push_back(layerName);
                while (iss >> layerName) {
                    renderOrder.push_back(layerName);
                }
            }
        } 
        else {
            Logging::err("Unknown property '" + l + "' in tilemap data file", "TileMap::loadPropertiesSection_");
            continue;
        }
    }

    if (mapSize.x <= 0 || mapSize.y <= 0) {
        Logging::err("Invalid map size in tilemap data file", "TileMap::loadPropertiesSection_");
    }

    return;
}

void TileMap::loadLayerSections_(const std::vector<std::vector<std::string>>& layerSections, const std::vector<std::string>& renderOrder,
                                 const std::unordered_map<int, std::tuple<TextureHandle, AnimationHandle, bool>>& tileTable,
                                 std::vector<std::vector<Tile>>& fullTilemap) {
    if (layerSections.empty()) {
        PRL::Logging::err("No layer sections found in tilemap data file", "TileMap::loadLayerSections_");
        return;
    }

    if (renderOrder.size() != layerSections.size()) {
        Logging::err("Number of layers in render order does not match number of layer sections in tilemap data file", "TileMap::loadLayerSections_");
        return;
    }

    if (fullTilemap.size() > 0) {
        fullTilemap.clear();
    }

    // Organize layer sections by layer name
    std::unordered_map<std::string, std::vector<std::string>> layerSectionsMap;
    for (size_t i(0); i < layerSections.size(); ++i) {
        if (layerSections[i].empty()) {
            Logging::err("Empty layer section found in tilemap data file", "TileMap::loadLayerSections_");
            continue;
        }
        auto layerNameIndex = std::find(layerSections[i].begin(), layerSections[i].end(), "layer_name");
        if (layerNameIndex == layerSections[i].end()) {
            Logging::err("Current layer section does not contain a line starting with 'layer_name' in tilemap data file", "TileMap::loadLayerSections_");
            continue;
        }
        else {
            // Check if layer name already added to map
            std::string layerName = *(layerNameIndex + 1);
            if (layerSectionsMap.find(layerName) != layerSectionsMap.end()) {
                Logging::err("Duplicate layer name '" + layerName + "' in tilemap data file", "TileMap::loadLayerSections_");
                continue;
            }
            else {
                for (size_t j(0); j < layerSections[i].size(); ++j) {
                    // if line does not contain layer_name, add it
                    if (layerSections[i][j].find("layer_name") == std::string::npos) {
                        layerSectionsMap[layerName].push_back(layerSections[i][j]);
                    }
                }
            }
        }
    }

    // Check that layer names in render order are the same as layer names in layer sections
    for (const auto& layerName : renderOrder) {
        if (layerSectionsMap.find(layerName) == layerSectionsMap.end()) {
            Logging::err("Layer name '" + layerName + "' in render order not found in layer sections of tilemap data file", "TileMap::loadLayerSections_");
            continue;
        }
    }

    // Now everything is good so that the full tilemap can be constructed
    for (size_t i(0); i < renderOrder.size(); ++i) {
        std::string layerName = renderOrder[i];
        const auto& layerLines = layerSectionsMap[layerName];
        fullTilemap.emplace_back(std::vector<Tile>(mapSize.x * mapSize.y)); // start a new layer in full tilemap
        
        int nTilesX(0), nTilesY(0);
        int tileID(-1);
        for (const auto& l : layerLines) {
            std::istringstream iss(l);
            nTilesX = 0;
            while (iss >> tileID) {
                // Check limits of tileID with respect to its type representation
                if (tileID < std::numeric_limits<int16_t>::min() || tileID > std::numeric_limits<int16_t>::max()) {
                    Logging::err("Tile ID " + std::to_string(tileID) + " out of range for int16_t in tilemap data file", "TileMap::loadLayerSections_");
                    continue;
                }
                TextureHandle texHandle = std::get<0>(tileTable.at(tileID));
                AnimationHandle animHandle = std::get<1>(tileTable.at(tileID));
                bool animated = std::get<2>(tileTable.at(tileID));
                auto& currentTile = fullTilemap.back()[nTilesX + nTilesY * mapSize.x];
                currentTile.ID = tileID;
                currentTile.textureHandle = texHandle;
                currentTile.animationHandle = animHandle;
                currentTile.animated = animated;
                nTilesX++;
            }
            if (nTilesX != mapSize.x) {
                Logging::err("Number of tiles in x direction does not match map size in tilemap data file (" + 
                    std::to_string(nTilesX) + " vs expected " + std::to_string(mapSize.x) + ")", "TileMap::loadLayerSections_");
            }
            nTilesY++;
        }
        if (nTilesY != mapSize.y) {
            Logging::err("Number of tiles in y direction does not match map size in tilemap data file (" + 
                std::to_string(nTilesY) + " vs expected " + std::to_string(mapSize.y) + ")", "TileMap::loadLayerSections_");
        }
    }
}


void TileMap::loadTableSection_(const std::vector<std::string>& lines, std::unordered_map<int, std::tuple<TextureHandle, AnimationHandle, bool>>& tileTable) {
    // temp
    return;
    if (lines.empty()) {
        PRL::Logging::err("No lines in [table] section of tilemap data file", "TileMap::loadTableSection_");
        return;
    }

    if (!tileTable.empty()) {
        tileTable.clear();
    }

    int tileID;
    std::string discard, assetName;
    bool animated;
    for (const auto& l : lines) {
        std::istringstream iss(l);

        if (!(iss >> tileID >> discard >> animated >> assetName)) {
            PRL::Logging::err("Unable to read all 4 mandatory fields from [table] section of tilemap data file", "PRL::TileMap::loadTableSection_");
        }
        TextureHandle textureHandle({0});
        AnimationHandle animHandle({0});
        if (animated) {
            animHandle = PRL::Core::getAssetManager().getAnimationHandle(assetName);
        } else {
            textureHandle = PRL::Core::getAssetManager().getTextureHandle(assetName);
        }
        tileTable.emplace(tileID, std::make_tuple(textureHandle, animHandle, animated));
    }

    // Check that all tiles have the same size, and report it
    Vec2D<int> tileSize(0, 0);
    for (size_t i(0); i < tileTable.size(); ++i) {
        const auto& entry = tileTable.at(i);
        const auto& textureHandle = std::get<0>(entry);
        const auto& animHandle = std::get<1>(entry);
        const TextureAsset* textureAsset = nullptr;
        const AnimationAsset* animAsset = nullptr;

        if (std::get<2>(entry)) {
            animAsset = PRL::Core::getAssetManager().getAnimationAsset(animHandle);
        }
        else {
            textureAsset = PRL::Core::getAssetManager().getTextureAsset(textureHandle);
        }

        if (i == 0) {
            tileSize = {0,0};
        }
    }
}

void TileMap::loadCollisionSection_(const std::vector<std::string>& lines) {
    if (lines.empty())
        return;

}






////// Old implementation
// extern EntityManager entityManager;

// TileMap::TileMap() :
//     mapSizeX_(0), mapSizeY_(0), tileSize_(0), mapScale_(0.0f),
//     scaledSize_(0.0f), tileSetTextureID_("") {
// }

// TileMap::TileMap(const std::string& tileSetTextureID, float mapScale) :
//     mapSizeX_(0), mapSizeY_(0), tileSize_(0), mapScale_(mapScale),
//     scaledSize_(0.0f), tileSetTextureID_(tileSetTextureID) {
//     assert(mapScale > 0.0f);
// }

// void TileMap::loadMap(const std::string& fileName, int tileSetTilesCountX) {
//     assert(tileSetTilesCountX > 0);

//     Logging::log("Loading map file: " + fileName, "PRL::TileMap::loadMap()");
//     std::ifstream mapFile(fileName, std::ios::in);
//     if (!mapFile.is_open()) {
//         Logging::err("Unable to open map file: " + fileName, "PRL::TileMap::loadMap()");
//     }

//     // Read the first line to get the size of the map and collision layer
//     int collLayer;
//     mapFile >> mapSizeX_; mapFile.ignore(); // ignore comma
//     mapFile >> mapSizeY_; mapFile.ignore();
//     mapFile >> tileSize_; mapFile.ignore();
//     mapFile >> collLayer;  // 0 or 1 for absence / presence of collision layer

//     if (mapSizeX_ <= 0 || mapSizeY_ <= 0) {
//         Logging::err("Invalid map size in file: " + fileName, "PRL::TileMap::loadMap()");
//     }
//     if (tileSize_ <= 0) {
//         Logging::err("Invalid tile size in file: " + fileName, "PRL::TileMap::loadMap()");
//     }

//     scaledSize_ = static_cast<float>(tileSize_) * mapScale_;
//     tileIDMap_ = std::vector<std::vector<int>>(mapSizeX_, std::vector<int>(mapSizeY_));
//     mapFile.ignore(); // ignore empty line

//     // Load the map
//     for (int y(0); y < mapSizeY_; ++y) {
//         for (int x(0); x < mapSizeX_; ++x) {
//             int tileID;
//             mapFile >> tileID;
//             tileIDMap_[x][y] = tileID;
//             mapFile.ignore();  // ignore comma or \n
//         }
//     }

//     if (collLayer == 0) {
//         collLayer_ = std::vector<std::vector<uint8_t>>(0, std::vector<uint8_t>(0));
//     } else {
//         collLayer_ = std::vector<std::vector<uint8_t>>(mapSizeX_, std::vector<uint8_t>(mapSizeY_));
//         mapFile.ignore(); // ignore empty line
//         for (int x(0); x < mapSizeX_; ++x) {
//             for (int y(0); y < mapSizeY_; ++y) {
//                 int collID;
//                 mapFile >> collID;
//                 collLayer_[x][y] = static_cast<uint8_t>(collID);
//                 mapFile.ignore();  // ignore comma or \n
//             }
//         }
//     }

//     // Load the tiles
//     for (int x(0); x < mapSizeX_; ++x) {
//         for (int y(0); y < mapSizeY_; ++y) {
//             if (tileIDMap_[x][y] < 0) continue; // Skip empty tiles
//             int srcX = (tileIDMap_[x][y] % tileSetTilesCountX) * tileSize_;
//             int srcY = (tileIDMap_[x][y] / tileSetTilesCountX) * tileSize_;
//             PosType posX = static_cast<PosType>(x * scaledSize_);
//             PosType posY = static_cast<PosType>(y * scaledSize_);
//             int collID = (collLayer_.size() == 0) ? 0 : collLayer_[x][y];
//             addTile_(srcX, srcY, posX, posY, collID);
//         }
//     }

//     Logging::log("Finished loading map file: " + fileName, "PRL::TileMap::loadMap()");
// }

// void TileMap::addTile_(int srcX, int srcY, PosType x, PosType y, int collID) {
//     auto &tile(entityManager.addEntity());
//     tile.addComponent<TileComponent>(srcX, srcY, x, y, tileSize_, mapScale_, tileSetTextureID_);
//     tile.addGroup(Game::groupMap);

//     if (collID != 0) {
//         // tile.addComponent<ColliderComponent>("terrain", x, y, scaledSize_);
//         tile.addGroup(Game::groupColliders);
//     }
// }

} // namespace PRL