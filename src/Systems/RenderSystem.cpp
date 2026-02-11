#include "Systems/RenderSystem.hpp"
#include "TextureManager.hpp"

RenderSystem::RenderSystem() {
    signature_.set(getComponentTypeID<SpriteComponent>());
}


void RenderSystem::draw() {
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