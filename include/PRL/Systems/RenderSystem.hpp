#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "Systems.hpp"
#include "ECS/SpriteComponent.hpp"
#include "ECS/TransformComponent.hpp"

class RenderSystem : public System {
public:
    RenderSystem();
    RenderSystem(EntityManager* entityManager, ComponentSignature signature = 
        ComponentSignature::create<SpriteComponent, TransformComponent>());
    ~RenderSystem();
    
    void update() override;
    void draw() override;

    static inline size_t getInstanceCount() noexcept { return instanceCount_; }

private:
    static size_t instanceCount_;
};

#endif // RENDER_SYSTEM_HPP