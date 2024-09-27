#ifndef _COLLIDER_COMPONENT_HPP_INCLUDED
#define _COLLIDER_COMPONENT_HPP_INCLUDED

#include <SDL2/SDL.h>
#include <string>
#include "../Types.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class ColliderComponent : public Component {
public:
    ColliderComponent();
    ColliderComponent(const std::string& tag);
    ColliderComponent(const std::string& tag, PosType x, PosType y, int size);

    void init() override;
    void update() override;
    void draw() override;
    
    PRL_Rect collider;
    std::string tag;
    TransformComponent* transform;

private:
    SDL_Texture* texture_;
    SDL_FRect dstRect_;
    SDL_Rect srcRect_;
};


#endif // _COLLIDER_COMPONENT_HPP_INCLUDED