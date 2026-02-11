#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "Systems.hpp"
#include "../ECS/SpriteComponent.hpp"

class RenderSystem : public System {
public:
    RenderSystem();
    ~RenderSystem() {}
    
    void update() override {}
    void draw() override;
};

#endif // RENDER_SYSTEM_HPP