#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "Systems.hpp"
#include "../ECS/SpriteComponent.hpp"

class RenderSystem : public System {
public:
    RenderSystem();
    RenderSystem(EntityManager* entityManager, 
        ComponentSignature signature = ComponentSignature::create<SpriteComponent>());
    ~RenderSystem() {}
    
    void update() override;
    void draw() override;

private:

};

#endif // RENDER_SYSTEM_HPP