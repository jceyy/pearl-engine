#include <fstream>
#include <cassert>
#include "Types.hpp"
#include "TileMap.hpp"
#include "Game.hpp"
#include "Logging.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"

using namespace PRL;
extern EntityManager manager;

TileMap::TileMap() :
    mapSizeX_(0), mapSizeY_(0), tileSize_(0), mapScale_(0.0f),
    scaledSize_(0.0f), tileSetTextureID_("") {
}

TileMap::TileMap(const std::string& tileSetTextureID, float mapScale) :
    mapSizeX_(0), mapSizeY_(0), tileSize_(0), mapScale_(mapScale),
    scaledSize_(0.0f), tileSetTextureID_(tileSetTextureID) {
    assert(mapScale > 0.0f);
}

void TileMap::loadMap(const std::string& fileName, int tileSetTilesCountX) {
    assert(tileSetTilesCountX > 0);

    Logging::log("Loading map file: " + fileName, "PRL::TileMap::loadMap()");
    std::ifstream mapFile(fileName, std::ios::in);
    if (!mapFile.is_open()) {
        Logging::err("Unable to open map file: " + fileName, "PRL::TileMap::loadMap()");
    }

    // Read the first line to get the size of the map and collision layer
    int collLayer;
    mapFile >> mapSizeX_; mapFile.ignore(); // ignore comma
    mapFile >> mapSizeY_; mapFile.ignore();
    mapFile >> tileSize_; mapFile.ignore();
    mapFile >> collLayer;  // 0 or 1 for absence / presence of collision layer

    if (mapSizeX_ <= 0 || mapSizeY_ <= 0) {
        Logging::err("Invalid map size in file: " + fileName, "PRL::TileMap::loadMap()");
    }
    if (tileSize_ <= 0) {
        Logging::err("Invalid tile size in file: " + fileName, "PRL::TileMap::loadMap()");
    }

    scaledSize_ = static_cast<float>(tileSize_) * mapScale_;
    tileIDMap_ = std::vector<std::vector<int>>(mapSizeX_, std::vector<int>(mapSizeY_));
    mapFile.ignore(); // ignore empty line

    // Load the map
    for (int y(0); y < mapSizeY_; ++y) {
        for (int x(0); x < mapSizeX_; ++x) {
            int tileID;
            mapFile >> tileID;
            tileIDMap_[x][y] = tileID;
            mapFile.ignore();  // ignore comma or \n
        }
    }

    if (collLayer == 0) {
        collLayer_ = std::vector<std::vector<uint8_t>>(0, std::vector<uint8_t>(0));
    } else {
        collLayer_ = std::vector<std::vector<uint8_t>>(mapSizeX_, std::vector<uint8_t>(mapSizeY_));
        mapFile.ignore(); // ignore empty line
        for (int x(0); x < mapSizeX_; ++x) {
            for (int y(0); y < mapSizeY_; ++y) {
                int collID;
                mapFile >> collID;
                collLayer_[x][y] = static_cast<uint8_t>(collID);
                mapFile.ignore();  // ignore comma or \n
            }
        }
    }

    // Load the tiles
    for (int x(0); x < mapSizeX_; ++x) {
        for (int y(0); y < mapSizeY_; ++y) {
            if (tileIDMap_[x][y] < 0) continue; // Skip empty tiles
            int srcX = (tileIDMap_[x][y] % tileSetTilesCountX) * tileSize_;
            int srcY = (tileIDMap_[x][y] / tileSetTilesCountX) * tileSize_;
            PosType posX = static_cast<PosType>(x * scaledSize_);
            PosType posY = static_cast<PosType>(y * scaledSize_);
            int collID = (collLayer_.size() == 0) ? 0 : collLayer_[x][y];
            addTile_(srcX, srcY, posX, posY, collID);
        }
    }

    Logging::log("Finished loading map file: " + fileName, "PRL::TileMap::loadMap()");
}

void TileMap::addTile_(int srcX, int srcY, PosType x, PosType y, int collID) {
    auto &tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, x, y, tileSize_, mapScale_, tileSetTextureID_);
    tile.addGroup(Game::groupMap);

    if (collID != 0) {
        // tile.addComponent<ColliderComponent>("terrain", x, y, scaledSize_);
        tile.addGroup(Game::groupColliders);
    }
}