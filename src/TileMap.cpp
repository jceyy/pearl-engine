#include <fstream>
#include <sstream>
#include <cassert>
#include "Core.hpp"
#include "Types.hpp"
#include "TileMap.hpp"
#include "Logging.hpp"

using namespace std;

namespace PRL {

size_t TileMap::instanceCount_ = 0;

TileMap::TileMap() : tileSize_(0, 0), mapSize_(0, 0), chunkCount_(0, 0), layers_(), chunks_(),
    textureName_(""), hasCollisionLayer_(false) {
        instanceCount_++;
}

TileMap::~TileMap() {
    instanceCount_--;
}

void TileMap::loadMap(const std::string& fileName) {
    if (layers_.size() > 0)
        layers_.clear();
    if (chunks_.size() > 0)
        chunks_.clear();

    std::ifstream mapStream(fileName);
    if (!mapStream) {
        Logging::err("Could not open map file: " + fileName, "PRL::TileMap::loadMap()");
        return;
    }

    Logging::log("Loading tilemap from file : " + fileName, "PRL::TileMap::loadMap()");
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
        Logging::err("No section found in tilemap data file : " + fileName, "PRL::TileMap::loadMap()");
        return;
    }
    if (layerSections.empty()) {
        Logging::err("No layer sections found in tilemap data file : " + fileName, "PRL::TileMap::loadMap()");
        return;
    }

    // Now load data from sections
    // AssetManager& assetManager = PRL::Core::getAssetManager();
    std::vector<std::string> renderOrder;
    std::vector<std::vector<Tile>> fullTilemap;
    
    // Load all the sections of the map file
    loadPropertiesSection_(sectionLines["[properties]"], renderOrder);
    loadTableSection_(sectionLines["[table]"]);
    loadLayerSections_(layerSections, renderOrder, fullTilemap);
    loadCollisionSection_(sectionLines["[collision]"]);
    
    // Create chunks
    constructChunks_(fullTilemap);
    return;
}


void TileMap::loadPropertiesSection_(const std::vector<std::string>& lines, std::vector<std::string>& renderOrder) {
    if (lines.empty()) {
        PRL::Logging::err("No lines in [properties] section of tilemap data file", "PRL::TileMap::loadPropertiesSection_()");
        return;
    }
    if (renderOrder.size() > 0) {
        renderOrder.clear();
    }

    std::string discard, layerName;
    mapSize_ = Vec2D<int>(0, 0);
    for (const auto& l : lines) {
        std::istringstream iss(l);
        if (l.find("map_size") == 0) {
            if (!(iss >> discard >> mapSize_.x >> mapSize_.y)) {
                Logging::err("Invalid map size format in tilemap data file", "PRL::TileMap::loadPropertiesSection_()");
                continue;
            }
        } else if (l.find("render_order") == 0) {
            if (!(iss >> discard >> layerName)) {
                Logging::err("Invalid render order format in tilemap data file", "PRL::TileMap::loadPropertiesSection_()");
                continue;
            }
            else {
                renderOrder.push_back(layerName);
                while (iss >> layerName) {
                    renderOrder.push_back(layerName);
                }
            }
        } else if (l.find("texture_name") == 0) {
            if (!(iss >> discard >> textureName_)) {
                Logging::err("Invalid texture name format in tilemap data file", "PRL::TileMap::loadPropertiesSection_()");
                continue;
            }
        } else {
            Logging::err("Unknown property '" + l + "' in tilemap data file", "PRL::TileMap::loadPropertiesSection_()");
            continue;
        }
    }

    if (mapSize_.x <= 0 || mapSize_.y <= 0) {
        Logging::err("Invalid map size in tilemap data file", "PRL::TileMap::loadPropertiesSection_()");
    }
    if (renderOrder.empty()) {
        Logging::err("No render order specified in tilemap data file", "PRL::TileMap::loadPropertiesSection_()");
    }

    // Check if the texture name is valid
    if (textureName_.empty() ) {
        Logging::err("Texture name not specified in tilemap data file", "PRL::TileMap::loadPropertiesSection_()");
    }
    if (Core::getAssetManager().hasTexture(textureName_) == false) {
        Logging::err("Texture '" + textureName_ + "' specified in tilemap data file not found in AssetManager", "PRL::TileMap::loadPropertiesSection_()");
    }
    return;
}


void TileMap::loadTableSection_(const std::vector<std::string>& lines) {
    if (lines.empty()) {
        PRL::Logging::err("No lines in [table] section of tilemap data file", "PRL::TileMap::loadTableSection_()");
        return;
    }

    long long tileID;
    std::string discard, regionName, animName;
    bool animated;
    for (const auto& l : lines) {
        std::istringstream iss(l);
        // # <tileref> texture   anim <0|1> <animation>
        if (!(iss >> tileID >> regionName >> discard >> animated)) {
            PRL::Logging::err("Unable to read all 4 mandatory fields from [table] section of tilemap data file", "PRL::TileMap::loadTableSection_()");
        }

        // tile ID
        if (tileID < Tile::minTileID || tileID > Tile::maxTileID) {
            Logging::err("Tile ID " + std::to_string(tileID) + " out of range (" 
                + std::to_string(Tile::minTileID) + " to " + std::to_string(Tile::maxTileID) 
                + ") for TileID in tilemap data file", "TileMap::loadTableSection_()");
            return;
        }

        // Region name to ID
        TextureHandle textureHandle = PRL::Core::getAssetManager().getTextureHandle(textureName_);
        TextureRegionID regionID = PRL::Core::getAssetManager().getTextureAsset(textureHandle)->regionNames.at(regionName);

        // Animation
        AnimationHandle animHandle({0});
        if (animated) {
            if (!(iss >> animName)) {
                PRL::Logging::err("Animated tile entry in [table] section of tilemap data file missing animation name", "PRL::TileMap::loadTableSection_()");
            }
            animHandle = PRL::Core::getAssetManager().getAnimationHandle(animName);
        }

        // Create definition slot if it does not exist, and assign tile data to it
        if (static_cast<size_t>(tileID) >= tileDefinitions_.size()) {
            tileDefinitions_.resize(tileID + 1, TileDefinition());
        }
        tileDefinitions_[tileID].texture = textureHandle;
        tileDefinitions_[tileID].animation = animHandle;
        tileDefinitions_[tileID].textureRegion = regionID;
        tileDefinitions_[tileID].animated = animated;
        tileDefinitions_[tileID].inUse = true;
    }

    // Check that all tiles have the same size, and report it
    Vec2D<int> tileSize(0, 0);
    for (size_t i(0); i < tileDefinitions_.size(); ++i) {
        const auto& def = tileDefinitions_[i];
        if (!def.inUse) continue;

        const TextureAsset* textureAsset = Core::getAssetManager().getTextureAsset(def.texture);
        if (i == 0) {
            tileSize.x = textureAsset->regions[def.textureRegion].w;
            tileSize.y = textureAsset->regions[def.textureRegion].h;
            continue;
        }

        if (textureAsset->regions[def.textureRegion].w != tileSize.x || 
            textureAsset->regions[def.textureRegion].h != tileSize.y) {
            PRL::Logging::err("Tile ID " + std::to_string(i) + " has different tile size than previous tiles in [table] section of tilemap data file", "PRL::TileMap::loadTableSection_()");
        }  
    }
    tileSize_ = tileSize;
}


void TileMap::loadLayerSections_(std::vector<std::vector<std::string>>& layerSections, const std::vector<std::string>& renderOrder,
                                 std::vector<std::vector<Tile>>& fullTilemap) {
    if (layerSections.empty()) {
        PRL::Logging::err("No layer sections found in tilemap data file", "PRL::TileMap::loadLayerSections_()");
    }

    if (renderOrder.size() != layerSections.size()) {
        Logging::err("Number of layers in render order does not match number of layer sections in tilemap data file", "PRL::TileMap::loadLayerSections_()");
    }

    if (fullTilemap.size() > 0) {
        fullTilemap.clear();
    }

    // Organize layer sections by layer name
    std::unordered_map<std::string, std::vector<std::string>> layerSectionsMap;
    for (size_t i(0); i < layerSections.size(); ++i) {
        if (layerSections[i].empty()) {
            Logging::err("Empty layer section found in tilemap data file", "PRL::TileMap::loadLayerSections_()");
        }

        auto& currentLayerLines = layerSections[i];
        std::string layerName("");
        size_t layerNameID;
        for (const auto& l : currentLayerLines) {
            if (l.find("layer_name") == 0) {
                // Get index of current line in layer section
                layerNameID = &l - &currentLayerLines[0];
                
                // Check if layer name already found in current layer section
                if (!layerName.empty()) {
                    Logging::err("Multiple 'layer_name' entries found in the same layer section of tilemap data file", "PRL::TileMap::loadLayerSections_()");
                }

                // Read layer name
                std::istringstream iss(l);
                std::string discard;
                if (!(iss >> discard >> layerName)) {
                    Logging::err("Invalid layer_name format in layer section of tilemap data file", "PRL::TileMap::loadLayerSections_()");
                }

                // Remove that line from the layer section lines, since it is not part of the tile data
                currentLayerLines.erase(currentLayerLines.begin() + layerNameID);
            }
        }

        if (layerName.empty()) {
            Logging::err("Layer section missing 'layer_name' entry in tilemap data file", "PRL::TileMap::loadLayerSections_()");
        }

        // Check that the layerName is not a duplicate of a previous layer name
        if (layerSectionsMap.size() > 0) {
            if (layerSectionsMap.find(layerName) != layerSectionsMap.end()) {
                Logging::err("Duplicate layer name '" + layerName + "' found in tilemap data file", "PRL::TileMap::loadLayerSections_()");
            }
        }
        layerSectionsMap.emplace(layerName, std::move(layerSections[i]));
    }

    // Check that layer names in render order are the same as layer names in layer sections
    for (const auto& layerName : renderOrder) {
        if (layerSectionsMap.find(layerName) == layerSectionsMap.end()) {
            Logging::err("Layer name '" + layerName + "' in render order not found in layer sections of tilemap data file", "PRL::TileMap::loadLayerSections_()");
            continue;
        }
    }

    // Now everything is good so that the full tilemap can be constructed
    for (size_t i(0); i < renderOrder.size(); ++i) {
        std::string layerName = renderOrder[i];
        layers_.emplace_back(TileLayer(layerName, true));
        const auto& layerLines = layerSectionsMap[layerName];
        fullTilemap.emplace_back(std::vector<Tile>(mapSize_.x * mapSize_.y)); // start a new layer in full tilemap
        
        int nTilesX(0), nTilesY(0);
        long long tileID(-1);
        for (const auto& l : layerLines) {
            std::istringstream iss(l);
            nTilesX = 0;
            while (iss >> tileID) {
                // Check limits of tileID with respect to its type representation
                if (tileID < 0 || tileID > Tile::maxTileID) {
                    tileID = Tile::emptyTileID; // assign empty tile ID for negative or out-of-range tile IDs
                }
                else if ((size_t)tileID >= tileDefinitions_.size() || !tileDefinitions_[tileID].inUse) {
                    Logging::err("Tile with ID " + std::to_string(tileID) + " at tile coordinates (" + 
                        std::to_string(nTilesX) + ", " + std::to_string(nTilesY) + ") in layer '" + layerName + 
                        "' is not defined in [table] section of tilemap data file", "PRL::TileMap::loadLayerSections_()");
                }

                fullTilemap.back()[(nTilesY * mapSize_.x) + nTilesX] = Tile(static_cast<TileID>(tileID));
                nTilesX++;
            }
            if (nTilesX != mapSize_.x) {
                Logging::err("Number of tiles in x direction does not match map size in tilemap data file (" + 
                    std::to_string(nTilesX) + " vs expected " + std::to_string(mapSize_.x) + ")", "PRL::TileMap::loadLayerSections_()");
            }
            nTilesY++;
        }
        if (nTilesY != mapSize_.y) {
            Logging::err("Number of tiles in y direction does not match map size in tilemap data file (" + 
                std::to_string(nTilesY) + " vs expected " + std::to_string(mapSize_.y) + ")", "PRL::TileMap::loadLayerSections_()");
        }
    }
}


void TileMap::loadCollisionSection_(const std::vector<std::string>& lines) {
    if (lines.empty()) {
        hasCollisionLayer_ = false;
    }
    // TO DO: implement the rest of the function when collision layer is needed
}

void TileMap::constructChunks_(const std::vector<std::vector<Tile>>& fullTilemap) {
    // Read through the whole tilemap and construct chunks on the fly

    // Prepare empty chunks
    const int NLayers = static_cast<int>(fullTilemap.size());
    const int NChunksX = ceil(static_cast<float>(mapSize_.x) / CHUNK_TILE_SIZE);
    const int NChunksY = ceil(static_cast<float>(mapSize_.y) / CHUNK_TILE_SIZE);
    chunkCount_ = Vec2D<int>(NChunksX, NChunksY);

    chunks_.reserve(NChunksX * NChunksY);

    // Loop through the whole tilemap and fill in chunk data
    for (int chunkY(0); chunkY < NChunksY; ++chunkY) {
        for (int chunkX(0); chunkX < NChunksX; ++chunkX) {
            TileChunk chunk(chunkX, chunkY);
            chunk.tiles.resize(NLayers);
            chunk.visible = true; // by default all chunks are visible, can be changed later for frustum culling or hiding
            
            for (int layer(0); layer < NLayers; ++layer) {
                std::vector<Tile>& tiles = chunk.tiles[layer];
                tiles.reserve(CHUNK_TILE_SIZE * CHUNK_TILE_SIZE);
                
                for (int localY(0); localY < CHUNK_TILE_SIZE; ++localY) {
                    for (int localX(0); localX < CHUNK_TILE_SIZE; ++localX) {
                        int globalX = (chunkX * CHUNK_TILE_SIZE) + localX;
                        int globalY = (chunkY * CHUNK_TILE_SIZE) + localY;
                        
                        if (globalX < mapSize_.x && globalY < mapSize_.y) {
                            tiles.push_back(fullTilemap[layer][globalY * mapSize_.x + globalX]);
                        }
                        else {
                            tiles.push_back(Tile(Tile::emptyTileID)); // fill out-of-bounds tiles with empty tiles
                        }
                    }
                }
            }
            chunks_.push_back(std::move(chunk));
        }
    }
}

} // namespace PRL