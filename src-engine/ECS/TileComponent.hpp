#ifndef TILE_COMPONENT_HPP
#define TILE_COMPONENT_HPP

#include <SDL2/SDL.h>
#include <string>
#include "ECS.hpp"
#include "../Types.hpp"

class TransformComponent;
class SpriteComponent;

class TileComponent : public Component{
public:
    TileComponent();
    TileComponent(int srcX, int srcY, PosType x, PosType y, int tileSize, float mapScale, const std::string& fileName);
    ~TileComponent();

    void init() override;
    void update() override;
    void draw() override;


    SDL_FRect dstRect_;
    Vector2D position;

private:
    // TransformComponent* transform_;
    // SpriteComponent* sprite_;
    // SDL_Rect tileRect_;
    // std::string fileName_;
    // int tileID_;
    
    SDL_Texture* texture_;
    SDL_Rect srcRect_;
    std::string fileName_;
};


#endif // TILE_COMPONENT_HPP