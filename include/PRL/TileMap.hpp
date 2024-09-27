#ifndef _TILE_MAP_HPP_INCLUDED
#define _TILE_MAP_HPP_INCLUDED
#include <string>
// #include <vector>
// #include <SDL2/SDL.h>
#include <string>

class TileMap{
public:
    TileMap();
    TileMap(const std::string& textureID, int tileSize, float mapScale);
    ~TileMap();

    void addTile(int srcX, int srcY, PosType x, PosType y);
    void loadMap(const std::string& fileName, int sizeX, int SizeY);

private:
    const std::string textureID_;
    float mapScale_;
    int tileSize_;
    float scaledSize_;

    // SDL_Rect src_;
    // SDL_FRect dst_;
    // SDL_Texture* dirt_;
    // SDL_Texture* grass_;
    // SDL_Texture* water_;

    // std::vector <std::vector<int>> map_;

};


#endif // _TILE_MAP_HPP_INCLUDED