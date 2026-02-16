#include "Systems/RenderSystem.hpp"
#include "TextureManager.hpp"

size_t RenderSystem::instanceCount_ = 0;

RenderSystem::RenderSystem(ComponentSignature signature) : 
System(signature) {
    instanceCount_++;
}

RenderSystem::~RenderSystem() {
    instanceCount_--;
}

void RenderSystem::draw() {
    std::cout << "[DEBUG] RenderSystem drawing entities\n";
    static int frameCount = 0;
    
    // Get entities from EntityManager that match this system's signature
    auto& entities = entityManager_->getEntitiesForSystem(SystemID::getSystemTypeID<RenderSystem>());
    if (true)
        std::cout << "[DEBUG] frame #" << frameCount++ << " : RenderSystem drawing " << entities.size() << " entities\n";
    
    SDL_FRect dst;
    SDL_Rect src;
    for (size_t i = 0; i < entities.size(); ++i) {
        Entity* entity = entities[i];
        if (!entity->getComponent<SpriteComponent>().visible) continue;

        dst.x = entity->getComponent<TransformComponent>().position.x;
        dst.y = entity->getComponent<TransformComponent>().position.y;
        TextureID textureID = entity->getComponent<SpriteComponent>().textureID;
        const TextureAsset& textureAsset = Game::assetManager->getTexture(textureID);

        Vector2D nativeSpriteSize = textureAsset.nativeSpriteSize;
        Vector2D scale = entity->getComponent<TransformComponent>().scale;
        dst.w = nativeSpriteSize.x * std::abs(scale.x);
        dst.h = nativeSpriteSize.y * std::abs(scale.y);
        size_t regionIndex = entity->getComponent<SpriteComponent>().region;
        src = { textureAsset.regions[regionIndex].x, textureAsset.regions[regionIndex].y, 
                textureAsset.regions[regionIndex].w, textureAsset.regions[regionIndex].h };
        
        TextureManager::Draw(
            textureAsset.texture, 
            &src, 
            &dst, entity->getComponent<SpriteComponent>().spriteFlip, entity->getComponent<TransformComponent>().rotation);
    }
}