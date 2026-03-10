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
        Tile() : ID(emptyTileID) {}
        Tile(TileID tileID) : ID(tileID) {}
        ~Tile() = default;
        
        TileID ID;

        constexpr static TileID minTileID = 0;
        constexpr static TileID maxTileID = std::numeric_limits<TileID>::max() - 1;
        constexpr static TileID emptyTileID = std::numeric_limits<TileID>::max();
    };

    class TileDefinition
    {
    public:
        TileDefinition() : texture({0}), animation({0}), textureRegion(0), flip(SDL_FLIP_NONE), animated(false), inUse(false) {}
        TileDefinition(bool animated, TextureHandle texture, TextureRegionID textureRegion, AnimationHandle animation)
            : texture(texture), animation(animation), textureRegion(textureRegion), flip(SDL_FLIP_NONE), animated(animated), inUse(true) {}
        ~TileDefinition() = default;

        TextureHandle texture;
        AnimationHandle animation;
        TextureRegionID textureRegion;
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
        bool visible;
    };

    class TileChunk {
    public:
        TileChunk() : visible(false), chunkCoord(0, 0), tiles() {}
        TileChunk(int chunkX, int chunkY) : visible(false), chunkCoord(chunkX, chunkY), tiles() {
            assert(chunkX >= 0 && chunkY >= 0);
        }
        
        //! \brief Whether the chunk should be rendered or not. Can be used for frustum culling or hiding chunks.
        bool visible;
        //! \brief Chunk coordinates in the chunk grid, not tile grid
        Vec2D<int> chunkCoord;
        //! \brief Tiles contained within the chunk, organized by layer
        std::vector<std::vector<Tile>> tiles;
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
        inline const TileChunk& getChunk(int chunkX, int chunkY) const;
        //! \brief Get chunk by tile coordinates
        inline TileChunk& getChunkFromTile(int tileX, int tileY);
        //! \brief Get chunk by world coordinates
        inline TileChunk& getChunkFromWorld(PosType worldX, PosType worldY);

        //! \brief Get tile by tile grid coordinates
        inline Tile& getTile(LayerID layer, int tileX, int tileY);
        //! \brief Get tile by world coordinates
        inline Tile& getTileFromWorld(LayerID layer, PosType worldX, PosType worldY);

        inline Vec2D<int> tileSize() const noexcept { return tileSize_; }
        inline Vec2D<int> mapSize() const noexcept { return mapSize_; }
        inline Vec2D<int> chunkCount() const noexcept { return chunkCount_; }

        static inline size_t instanceCount() noexcept { return instanceCount_; }
        
        //! \brief How many tiles of one chunk contains in each dimension. Should be a power of 2 for easier calculations.
        constexpr static int CHUNK_TILE_SIZE = 32;
        constexpr static int CHUNK_BIT_SHIFT = 5;
        constexpr static int CHUNK_TILE_MASK = CHUNK_TILE_SIZE - 1;
    
    private:
        void loadPropertiesSection_(const std::vector<std::string>& lines, std::vector<std::string>& renderOrder);
        void loadTableSection_(const std::vector<std::string>& lines);
        void loadLayerSections_(std::vector<std::vector<std::string>>& layerSections, 
            const std::vector<std::string>& renderOrder, std::vector<std::vector<Tile>>& fullTilemap);
        void loadCollisionSection_(const std::vector<std::string>& lines);
        void constructChunks_(const std::vector<std::vector<Tile>>& fullTilemap);

        std::vector<TileDefinition> tileDefinitions_; // temporary storage for tilemap data during loading, organized by layer and then by tile ID
        Vec2D<int> tileSize_;    //!< Size of one tile in pixels
        Vec2D<int> mapSize_;     //!< Size of the map in tiles
        Vec2D<int> chunkCount_;  //!< Size of the map in chunks, i.e. number of chunks in each dimension

        std::vector<TileLayer> layers_;
        std::vector<TileChunk> chunks_;
        std::string textureName_;
        bool hasCollisionLayer_;

        static size_t instanceCount_;

        friend class TileMapSystem;
    };



    // Inline definitions
    inline Tile& TileMap::getTile(LayerID layer, int tileX, int tileY) {
        assert(layer < layers_.size());
        assert(tileX >= 0 && tileX < mapSize_.x);
        assert(tileY >= 0 && tileY < mapSize_.y);

        int chunkX = tileX >> CHUNK_BIT_SHIFT;
        int chunkY = tileY >> CHUNK_BIT_SHIFT;

        // chunks are stored in a 1D vector, 2D maps to it reading left to right, top to bottom
        size_t chunkIndex = chunkY * chunkCount_.x + chunkX;
        assert(chunkIndex < chunks_.size());
        return chunks_[chunkIndex].tiles[layer][tileX + tileY * chunkCount_.x]; 
    }

    inline Tile& TileMap::getTileFromWorld(LayerID layer, PosType worldX, PosType worldY)
    {
        return getTile(layer, 
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

    inline const TileChunk& TileMap::getChunk(int chunkX, int chunkY) const {
        assert(chunkX >= 0 && chunkX < chunkCount_.x);
        assert(chunkY >= 0 && chunkY < chunkCount_.y);

        size_t chunkIndex = chunkY * chunkCount_.x + chunkX;
        return chunks_[chunkIndex];
    }

    TileChunk& TileMap::getChunkFromTile(int tileX, int tileY) {
        return getChunk(tileX >> CHUNK_BIT_SHIFT, tileY >> CHUNK_BIT_SHIFT);
    }

    inline TileChunk& TileMap::getChunkFromWorld(PosType worldX, PosType worldY) {
        return getChunk(static_cast<int>(worldX / (tileSize_.x << CHUNK_BIT_SHIFT)), 
                        static_cast<int>(worldY / (tileSize_.y << CHUNK_BIT_SHIFT)));
    }

} // namespace PRL

#endif // _TILEMAP_HPP_INCLUDED