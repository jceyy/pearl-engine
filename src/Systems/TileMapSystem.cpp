#include "Systems/TileMapSystem.hpp"
#include "TextureManager.hpp"

using namespace std;

namespace PRL {

size_t TileMapSystem::instanceCount_ = 0;

TileMapSystem::TileMapSystem(ComponentSignature signature) : 
System(signature) {
    systemName_ = "TileMapSystem";
    instanceCount_++;
}

TileMapSystem::~TileMapSystem() {
    instanceCount_--;
}

void TileMapSystem::update() {
    // Update logic for tile map system, such as animations or dynamic tile changes, would go here.
    // Also, which chunks are visible based on the camera position
    return;
}

void TileMapSystem::draw() {
    // Get entities from EntityManager that match this system's signature
    size_t renderSystemID = SystemID::getSystemTypeID<TileMapSystem>();
    auto& tileMaps = entityManager_->getEntitiesForSystem(renderSystemID);

    SDL_FRect dst;
    SDL_Rect src;
    for (size_t i = 0; i < tileMaps.size(); ++i) {
        const TileMap& tilemap = tileMaps[i]->getComponent<TileMapComponent>().map;

        for (size_t chunkIdx = 0; chunkIdx < tilemap.chunks_.size(); ++chunkIdx) {
            if (!tilemap.chunks_[chunkIdx].visible) continue; // skip invisible chunks

            const TileChunk& chunk = tilemap.chunks_[chunkIdx];
            for (size_t layerIdx = 0; layerIdx < chunk.tiles.size(); ++layerIdx) {
                const std::vector<Tile>& layer = chunk.tiles[layerIdx];

                for (int tileY = 0; tileY < TileMap::CHUNK_TILE_SIZE; ++tileY) {
                    for (int tileX = 0; tileX < TileMap::CHUNK_TILE_SIZE; ++tileX) {
                        const Tile& tile = layer[tileY * TileMap::CHUNK_TILE_SIZE + tileX];
                        if (tile.ID == Tile::emptyTileID) continue; // skip empty tiles

                        const TileDefinition& tileDef = tilemap.tileDefinitions_[tile.ID];
                        TextureHandle textureHandle = tileDef.texture;
                        const TextureAsset* textureAsset = assetManager_->getTextureAsset(textureHandle);

                        dst = {
                            static_cast<float>((chunk.chunkCoord.x + tilemap.tileSize_.x*tileX)),
                            static_cast<float>((chunk.chunkCoord.y + tilemap.tileSize_.y*tileY)),
                            static_cast<float>(tilemap.tileSize_.x),
                            static_cast<float>(tilemap.tileSize_.y)
                        };
                        
                        src = { 
                            textureAsset->regions[tileDef.textureRegion].x, 
                            textureAsset->regions[tileDef.textureRegion].y, 
                            textureAsset->regions[tileDef.textureRegion].w, 
                            textureAsset->regions[tileDef.textureRegion].h 
                        };

                        TextureManager::Draw(textureAsset->texture, &src, &dst, tileDef.flip, 0.0);
                    }
                }
            }
        }
    }
}


} // namespace PRL