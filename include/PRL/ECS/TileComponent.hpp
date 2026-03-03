#ifndef _TILE_COMPONENT_HPP_INCLUDED
#define _TILE_COMPONENT_HPP_INCLUDED

#include <SDL2/SDL.h>
#include "TileMap.hpp"
#include "ECS.hpp"
#include "../Types.hpp"

namespace PRL {
    class TileMapComponent : public Component {
    public:
        TileMapComponent() {}
        ~TileMapComponent() {} 

        TileMap map;

        static size_t getInstanceCount() noexcept { return instanceCount_; }
    
    private:
        static size_t instanceCount_;
    };


    // Old things -- to be removed
    class TransformComponent;
    class SpriteComponent;

    class TileComponent : public Component{
    public:
        TileComponent();
        TileComponent(int srcX, int srcY, PosType x, PosType y, int tileSize, float mapScale, const std::string& textureID);
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
        std::string textureID_;
    };

} // namespace PRL

#endif // _TILE_COMPONENT_HPP_INCLUDED