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

    class Tile {
    public:
        Tile() : ID(0) {}
        Tile(TileID ID) : ID(ID) {}
        ~Tile() = default;
        
        TileID ID;

        constexpr static TileID maxTileID = std::numeric_limits<TileID>::max() - 1;
        constexpr static TileID emptyTileID = std::numeric_limits<TileID>::max();
    };

    class TileDefinition
    {
    public:
        TileDefinition() : texture({0}), animation({0}), textureRegion(0), animated(false), inUse(false) {}
        TileDefinition(bool animated, TextureHandle texture, uint16_t textureRegion, AnimationHandle animation)
            : texture(texture), animation(animation), textureRegion(textureRegion), animated(animated), inUse(true) {}
        ~TileDefinition() = default;

        TextureHandle texture;
        AnimationHandle animation;
        uint16_t textureRegion;
        SDL_RendererFlip flip;
        bool animated;
        bool inUse;
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
        TileChunk() : chunkCoord(0, 0), layerTiles() {}
        TileChunk(int chunkX, int chunkY) : chunkCoord(chunkX, chunkY), layerTiles() {
            assert(chunkX >= 0 && chunkY >= 0);
        }
        
        //! \brief Chunk coordinates in the chunk grid, not tile grid
        Vec2D<int> chunkCoord;

        // one tile array per layer
        std::vector<std::vector<Tile>> layerTiles;
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
        inline TileChunk& getChunk(int chunkX, int chunkY);

        //! \brief Get tile by tile grid coordinates
        inline Tile& atGrid(LayerID layer, int x, int y);
        
        //! \brief Get tile by world coordinates
        inline Tile& atWorld(LayerID layer, PosType worldX, PosType worldY);

        inline Vec2D<int> tileSize() const noexcept { return tileSize_; }
        inline Vec2D<int> mapSize() const noexcept { return mapSize_; }

        static inline size_t instanceCount() noexcept { return instanceCount_; }
        
        //! \brief How many tiles of one chunk contains in each dimension. Should be a power of 2 for easier calculations.
        constexpr static int TILE_CHUNK_SIZE = 32;
    
    private:
        void loadPropertiesSection_(const std::vector<std::string>& lines, std::vector<std::string>& renderOrder);
        void loadTableSection_(const std::vector<std::string>& lines);
        void loadLayerSections_(const std::vector<std::vector<std::string>>& layerSections, 
            const std::vector<std::string>& renderOrder, const std::unordered_map<int, std::tuple<TextureHandle, AnimationHandle, bool>>& tileTable, 
            std::vector<std::vector<Tile>>& fullTilemap);
        void loadCollisionSection_(const std::vector<std::string>& lines);

        std::vector<TileDefinition> tileDefinitions_; // temporary storage for tilemap data during loading, organized by layer and then by tile ID
        Vec2D<int> tileSize_;    //!< Size of one tile in pixels
        Vec2D<int> mapSize_;     //!< Size of the map in tiles
        Vec2D<int> chunkCount_;  //!< Size of the map in chunks, i.e. number of chunks in each dimension

        std::vector<TileLayer> layers_;
        std::vector<TileChunk> chunks_;
        TextureID textureName_;

        static size_t instanceCount_;
    };

    // Inline definitions
    inline Tile& TileMap::atGrid(LayerID layer, int x, int y) {
        assert(layer < layers_.size());
        assert(x >= 0 && x < mapSize_.x);
        assert(y >= 0 && y < mapSize_.y);

        // chunks are stored in a 1D vector, 2D maps to it reading left to right, top to bottom
        size_t chunkIndex = (y / TILE_CHUNK_SIZE) * chunkCount_.x + (x / TILE_CHUNK_SIZE);
        return chunks_[chunkIndex].layerTiles[layer][x + y * chunkCount_.x]; 
    }

    inline Tile& TileMap::atWorld(LayerID layer, PosType worldX, PosType worldY)
    {
        return atGrid(layer, 
            static_cast<int>(worldX / tileSize_.x),
            static_cast<int>(worldY / tileSize_.y)
        );
    }

    inline TileChunk& TileMap::getChunk(int chunkX, int chunkY) {
            assert(chunkX >= 0 && chunkX < chunkCount_.x);
            assert(chunkY >= 0 && chunkY < chunkCount_.y);

            size_t chunkIndex = chunkY * chunkCount_.x + chunkX;
            return chunks_[chunkIndex];
        }

} // namespace PRL

#endif // _TILEMAP_HPP_INCLUDED