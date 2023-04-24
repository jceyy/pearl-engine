#include <fstream>
#include "Types.hpp"
#include "TileMap.hpp"
#include "Game.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"

extern EntityManager manager;

TileMap::TileMap() {}

TileMap::TileMap(const std::string& fileName, int tileSize, float mapScale)
: fileName_(fileName), mapScale_(mapScale), tileSize_(tileSize), 
scaledSize_(mapScale * tileSize) {
}


TileMap::~TileMap() {}

void TileMap::loadMap(const std::string& fileName, int sizeX, int sizeY) {
    // map_.clear();
    char c;
    int srcX, srcY;
    std::fstream mapFile;
    mapFile.open(fileName);

    for (size_t y(0); y < (size_t)sizeY; ++y) {
        for (size_t x(0); x < (size_t)sizeX; ++x) {
            mapFile.get(c);
            if (c == '\n' or c == '\r') mapFile.get(c);

            srcY = atoi(&c) * tileSize_;
            mapFile.get(c);
            srcX = atoi(&c) * tileSize_;
            addTile(srcX, srcY, static_cast<PosType>(x * scaledSize_), static_cast<PosType>(y * scaledSize_));
            mapFile.ignore();  // ignore comma
        }
    }
    
    mapFile.ignore();
    mapFile.ignore();
    
    // Collider
    for (size_t y(0); y < sizeY; ++y) {
        for (size_t x(0); x < sizeX; ++x) {
            mapFile.get(c);
            if (c == '\n' or c == '\r') mapFile.get(c);

            if (c == '1') {
                auto& tcol(manager.addEntity());
                tcol.addComponent<ColliderComponent>("terrain", x * scaledSize_, y * scaledSize_, scaledSize_);
                tcol.addGroup(Game::groupColliders);
            }
            mapFile.ignore();
        }
    }
    mapFile.close();
}

void TileMap::addTile(int srcX, int srcY, PosType x, PosType y){
    auto &tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, x, y, tileSize_, mapScale_, fileName_);
    tile.addGroup(Game::groupMap);
}