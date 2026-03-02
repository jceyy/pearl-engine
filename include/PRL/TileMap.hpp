#ifndef _TILEMAP_HPP_INCLUDED
#define _TILEMAP_HPP_INCLUDED

#include <string>
#include <vector>
#include <cassert>
#include "Types.hpp"
#include "AssetManager.hpp"

namespace PRL {

    //! \brief How many tiles of one chunk contains in each dimension. Should be a power of 2 for easier calculations.
    constexpr int TILE_CHUNK_SIZE = 32;

    // enum class TileFlags : uint8_t {
    //     None = 0,
    //     Animated = 1 << 0,
    //     Collidable = 1 << 1,
    //     // Add more flags as needed
    // };

    using TileID = int16_t;
    using LayerID = int8_t;

    class Tile {
    public:
        Tile() : ID(0), textureHandle({0}), animationHandle({0}), animated(false) {}
        
        TileID ID;
        TextureHandle textureHandle;
        AnimationHandle animationHandle;
        bool animated;
    };

    class TileLayer {
    public:
        TileLayer() : name(""), visible(true) {}
        TileLayer(const std::string& name, bool visible) : name(name), visible(visible) {}
        ~TileLayer() = default;

        std::string name;
        bool visible;
    };

    class TileChunk {
    public:
        TileChunk() : chunkCoord(0, 0), layerTiles(), dirty(true) {}
        
        //! \brief Chunk coordinates in the chunk grid, not tile grid
        Vec2D<int> chunkCoord;

        // one tile array per layer
        std::vector<std::vector<Tile>> layerTiles;

        bool dirty = true;
    };

    class TileMap : PRLObject {
    public:
        TileMap();
        ~TileMap();

        /** \brief Load the map from a file.
         * 
         * Details.
         */
        void loadMap(const std::string& fileName);

        //! \brief Get chunk by chunk coordinates
        inline TileChunk& getChunk(int chunkX, int chunkY) {
            assert(chunkX >= 0 && chunkX < chunkCount.x);
            assert(chunkY >= 0 && chunkY < chunkCount.y);

            size_t chunkIndex = chunkY * chunkCount.x + chunkX;
            return chunks[chunkIndex];
        }

        //! \brief Get tile by tile grid coordinates
        inline Tile& atGrid(LayerID layer, int x, int y) {
            assert(layer < layers.size());
            assert(x >= 0 && x < mapSize.x);
            assert(y >= 0 && y < mapSize.y);

            // chunks are stored in a 1D vector, 2D maps to it reading left to right, top to bottom
            size_t chunkIndex = (y / TILE_CHUNK_SIZE) * chunkCount.x + (x / TILE_CHUNK_SIZE);
            return chunks[chunkIndex].layerTiles[layer][x + y * chunkCount.x]; 
        }
        
        //! \brief Get tile by world coordinates
        inline Tile& atWorld(LayerID layer, PosType worldX, PosType worldY)
        {
            return atGrid(layer, 
                static_cast<int>(worldX / tileSize.x),
                static_cast<int>(worldY / tileSize.y)
            );
        }
        
        Vec2D<int> tileSize;    //!< Size of one tile in pixels
        Vec2D<int> mapSize;     //!< Size of the map in tiles
        Vec2D<int> chunkCount;  //!< Size of the map in chunks, i.e. number of chunks in each dimension

        std::vector<TileLayer> layers;
        std::vector<TileChunk> chunks;

        static inline size_t instanceCount() noexcept { return instanceCount_; }
    
    private:
        void loadPropertiesSection_(const std::vector<std::string>& lines, std::vector<std::string>& renderOrder);
        void loadTableSection_(const std::vector<std::string>& lines, std::unordered_map<int, std::tuple<TextureHandle, AnimationHandle, bool>>& tileTable);
        void loadLayerSections_(const std::vector<std::vector<std::string>>& layerSections, 
            const std::vector<std::string>& renderOrder, const std::unordered_map<int, std::tuple<TextureHandle, AnimationHandle, bool>>& tileTable, 
            std::vector<std::vector<Tile>>& fullTilemap);
        void loadCollisionSection_(const std::vector<std::string>& lines);

        static size_t instanceCount_;
    };

    ///// Old implementation
    // class TileMap {
    // public:
    //     TileMap();
    //     TileMap(const std::string& tileSetTextureID, float mapScale);
    //     ~TileMap() = default;

    //     void loadMap(const std::string& fileName, int tileSetTilesCountX); // Load the map from a file
    //     // Notes: the map file should be as follows (note no spaces):
    //     // nTilesX,nTilesY,tileSize,collLayer   <--- Size of the map, size of tile, and 0 or 1 for absence / presence of collision layer
    //     // tileID,tileID, ... (nTilesX)         <--- Start of Layer 0 (main layer)
    //     // tileID,tileID, ... (nTilesX)
    //     // ...
    //     // tileID,tileID, ... (nTilesX)         <--- End of Layer 0 (main layer)
    //     //                                      <--- Blank line
    //     // collID,collID, ... (nTilesX)         <--- Start of Layer 1 (collision layer)
    //     // collID,collID, ... (nTilesX)    
    //     // ...
    //     // collID,collID, ... (nTilesX)         <--- End of Layer 1 (collision layer)

    // private:
    //     void addTile_(int srcX, int srcY, PosType x, PosType y, int collID);

    //     int mapSizeX_;
    //     int mapSizeY_;
    //     int tileSize_;
    //     float mapScale_;
    //     float scaledSize_;
    //     const std::string tileSetTextureID_;
    //     std::vector<std::vector<int>> tileIDMap_;
    //     std::vector<std::vector<uint8_t>> collLayer_;
    // };

} // namespace PRL

#endif // _TILEMAP_HPP_INCLUDED