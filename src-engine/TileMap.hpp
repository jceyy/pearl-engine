#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP
#include <string>
// #include <vector>
// #include <SDL2/SDL.h>

class TileMap{
public:
    TileMap();
    ~TileMap();

    static void loadMap(const std::string& fileName, int sizeX, int SizeY);

private:
    // SDL_Rect src_;
    // SDL_FRect dst_;
    // SDL_Texture* dirt_;
    // SDL_Texture* grass_;
    // SDL_Texture* water_;

    // std::vector <std::vector<int>> map_;

};


#endif // TILE_MAP_HPP