#include "Systems/RenderSystem.hpp"
#include "TextureManager.hpp"

size_t RenderSystem::instanceCount_ = 0;
RenderSystem::RenderSystem() : System() {
    signature_ = ComponentSignature::create<SpriteComponent, TransformComponent>();
    instanceCount_++;
}

RenderSystem::RenderSystem(EntityManager* entityManager, ComponentSignature signature) : 
System(entityManager, signature) {
    assert(entityManager != nullptr);
    instanceCount_++;
}

RenderSystem::~RenderSystem() {
    instanceCount_--;
}

void RenderSystem::update() {
    return;
}

void RenderSystem::draw() {
    static int frameCount = 0;
    
    // Get entities from EntityManager that match this system's signature
    auto& entities = entityManager_->getEntitiesForSystem(SystemID::getSystemTypeID<RenderSystem>());
    // std::cout << "[DEBUG] frame #" << frameCount++ << " : RenderSystem drawing " << entities.size() << " entities\n";
    SDL_FRect dst;
    for (size_t i = 0; i < entities.size(); ++i) {
        Entity* entity = entities[i];
        dst.x = entity->getComponent<TransformComponent>().position.x;
        dst.y = entity->getComponent<TransformComponent>().position.y;
        dst.w = entity->getComponent<SpriteComponent>().dstRect_.w * entity->getComponent<TransformComponent>().scale.x;
        dst.h = entity->getComponent<SpriteComponent>().dstRect_.h * entity->getComponent<TransformComponent>().scale.y;
        
        TextureManager::Draw(
            entity->getComponent<SpriteComponent>().texture_, 
            &entity->getComponent<SpriteComponent>().srcRect_, 
            &dst, entity->getComponent<SpriteComponent>().spriteFlip_);
    }
}