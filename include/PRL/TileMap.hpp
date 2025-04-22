#ifndef _TILE_MAP_HPP_INCLUDED
#define _TILE_MAP_HPP_INCLUDED
#include <string>
#include <vector>

namespace PRL {

    class TileMap {
    public:
        TileMap();
        TileMap(const std::string& tileSetTextureID, float mapScale);
        ~TileMap() = default;

        void loadMap(const std::string& fileName, int tileSetTilesCountX); // Load the map from a file
        // Notes: the map file should be as follows (note no spaces):
        // nTilesX,nTilesY,tileSize,collLayer   <--- Size of the map, size of tile, and 0 or 1 for absence / presence of collision layer
        // tileID,tileID, ... (nTilesX)         <--- Start of Layer 0 (main layer)
        // tileID,tileID, ... (nTilesX)
        // ...
        // tileID,tileID, ... (nTilesX)         <--- End of Layer 0 (main layer)
        //                                      <--- Blank line
        // collID,collID, ... (nTilesX)         <--- Start of Layer 1 (collision layer)
        // collID,collID, ... (nTilesX)    
        // ...
        // collID,collID, ... (nTilesX)         <--- End of Layer 1 (collision layer)

    private:
        void addTile_(int srcX, int srcY, PosType x, PosType y, int collID);

        int mapSizeX_;
        int mapSizeY_;
        int tileSize_;
        float mapScale_;
        float scaledSize_;
        const std::string tileSetTextureID_;
        std::vector<std::vector<int>> tileIDMap_;
        std::vector<std::vector<uint8_t>> collLayer_;
    };

} // namespace PRL

#endif // _TILE_MAP_HPP_INCLUDED