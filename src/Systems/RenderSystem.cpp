#include "Systems/RenderSystem.hpp"
#include "TextureManager.hpp"

RenderSystem::RenderSystem() : System() {
    signature_.set(ComponentID::getComponentTypeID<SpriteComponent>());
}

RenderSystem::RenderSystem(EntityManager* entityManager, ComponentSignature signature) : 
System(entityManager, signature) {
    assert(entityManager != nullptr);
}

void RenderSystem::update() {
    return;
}

void RenderSystem::draw() {
    static int frameCount = 0;
    
    // Get entities from EntityManager that match this system's signature
    auto& entities = entityManager_->getEntitiesForSystem(SystemID::getSystemTypeID<RenderSystem>());
    std::cout << "[DEBUG] frame #" << frameCount++ << " : RenderSystem drawing " << entities.size() << " entities\n";
    SDL_FRect dst;
    for (size_t i = 0; i < entities.size(); ++i) {
        Entity* entity = entities[i];
        if (entity->hasComponent<SpriteComponent>()) {
            dst.x = SDL_floorf(entity->getComponent<SpriteComponent>().dstRect_.x + 0.0001f);
            dst.y = SDL_floorf(entity->getComponent<SpriteComponent>().dstRect_.y + 0.0001f);
            dst.w = entity->getComponent<SpriteComponent>().dstRect_.w;
            dst.h = entity->getComponent<SpriteComponent>().dstRect_.h;
            TextureManager::Draw(
                entity->getComponent<SpriteComponent>().texture_, 
                &entity->getComponent<SpriteComponent>().srcRect_, 
                &dst, entity->getComponent<SpriteComponent>().spriteFlip_);
        }
    }
}