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
    TileComponent(PosType x, PosType y, int w, int h, int id);

    void init() override;


private:
    TransformComponent* transform_;
    SpriteComponent* sprite_;
    SDL_Rect tileRect_;
    std::string fileName_;
    int tileID_;

};


#endif // TILE_COMPONENT_HPP