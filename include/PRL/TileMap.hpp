#ifndef _TILEMAP_HPP_INCLUDED
#define _TILEMAP_HPP_INCLUDED

#include <string>
#include <vector>
#include <cassert>
#include "Types.hpp"
#include "AssetManager.hpp"

namespace PRL {

    // enum class TileFlags : uint8_t {
    //     None = 0,
    //     Animated = 1 << 0,
    //     Collidable = 1 << 1,
    //     // Add more flags as needed
    // };

    using TileID = int16_t;
    using LayerID = uint8_t;

    class Tile {
    public:
        Tile() : ID(0) {}
        
        TileID ID;
        // TextureHandle textureHandle;
        // AnimationHandle animationHandle;
        // bool animated;
    };

    class TileLayer {
    public:
        TileLayer() : name(""), visible(true) {}
        TileLayer(const std::string& name, bool visible) : name(name), visible(visible) {}
        ~TileLayer() = default;

        std::string name;
        LayerID ID;
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
        
        //! \brief How many tiles of one chunk contains in each dimension. Should be a power of 2 for easier calculations.
        constexpr static int TILE_CHUNK_SIZE = 32;
    
    private:
        void loadPropertiesSection_(const std::vector<std::string>& lines, std::vector<std::string>& renderOrder);
        void loadTableSection_(const std::vector<std::string>& lines, std::unordered_map<int, std::tuple<TextureHandle, AnimationHandle, bool>>& tileTable);
        void loadLayerSections_(const std::vector<std::vector<std::string>>& layerSections, 
            const std::vector<std::string>& renderOrder, const std::unordered_map<int, std::tuple<TextureHandle, AnimationHandle, bool>>& tileTable, 
            std::vector<std::vector<Tile>>& fullTilemap);
        void loadCollisionSection_(const std::vector<std::string>& lines);

        static size_t instanceCount_;
    };

} // namespace PRL

#endif // _TILEMAP_HPP_INCLUDED