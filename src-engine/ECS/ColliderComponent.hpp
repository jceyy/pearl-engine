#ifndef COLLIDER_COMPONENT_HPP
#define COLLIDER_COMPONENT_HPP

#include <SDL2/SDL.h>
#include <string>
#include "../Types.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class ColliderComponent : public Component {
public:
    ColliderComponent();
    ColliderComponent(const std::string& tag);

    void init() override;
    void update() override;
    
    PRL_Rect collider;
    std::string tag;
    TransformComponent* transform;

private:
};


#endif // COLLIDER_COMPONENT_HPP