#include <fstream>
#include "TileMap.hpp"
#include "Game.hpp"


TileMap::TileMap() {
    // src_.x = src_.y = 0;
    // src_.w = src_.h = 32;
    // dst_.x = dst_.y = 0.0f;
    // dst_.w = dst_.h = tileSize;

    // dirt_ = TextureManager::loadTexture("assets/dirt.png");
    // grass_ = TextureManager::loadTexture("assets/grass.png");
    // water_ = TextureManager::loadTexture("assets/water.png");
}

TileMap::~TileMap() {}

void TileMap::loadMap(const std::string& fileName, int sizeX, int sizeY) {
    // map_.clear();
    char tile;
    std::fstream mapFile;
    mapFile.open(fileName);

    for (size_t y(0); y < (size_t)sizeY; ++y) {
        for (size_t x(0); x < (size_t)sizeX; ++x) {
            mapFile.get(tile);
            if (tile == '\n' or tile == '\r') mapFile.get(tile);
            Game::addTile((int) x * 32, (int) y * 32, atoi(&tile));
            mapFile.ignore();  // ignore comma
        }
    }
    mapFile.close();
}