#include "Systems/RenderSystem.hpp"
#include "TextureManager.hpp"
#include "Core.hpp"

using namespace std;

namespace PRL {

size_t RenderSystem::instanceCount_ = 0;

RenderSystem::RenderSystem(ComponentSignature signature) : 
System(signature) {
    systemName_ = "RenderSystem";
    instanceCount_++;
}

RenderSystem::~RenderSystem() {
    instanceCount_--;
}

void RenderSystem::draw() {
    // Get entities from EntityManager that match this system's signature
    size_t renderSystemID = SystemID::getSystemTypeID<RenderSystem>();
    auto& entities = entityManager_->getEntitiesForSystem(renderSystemID);

    const Camera& camera = Core::getCamera();
    const Vector2D camTopLeft = camera.topLeft();
    const Vec2D<int> logicalSize = Core::getLogicalSize();

    SDL_FRect dst;
    SDL_Rect src;
    for (size_t i = 0; i < entities.size(); ++i) {
        Entity* entity = entities[i];
        const TextureHandle& textureHandle = entity->getComponent<SpriteComponent>().textureHandle;
        const TextureAsset* textureAsset = assetManager_->getTextureAsset(textureHandle);
        int refPixelsPerUnit = Core::settings.render.pixelsPerUnit;
        int pixelsPerUnit = textureAsset->pixels_per_unit;
        float pxlUnitScale = static_cast<float>(refPixelsPerUnit) / static_cast<float>(pixelsPerUnit);
        if (!entity->getComponent<SpriteComponent>().visible) continue;
        // World -> screen: offset by the camera's top-left corner.
        dst.x = (entity->getComponent<TransformComponent>().position.x - camTopLeft.x) * refPixelsPerUnit;
        dst.y = (entity->getComponent<TransformComponent>().position.y - camTopLeft.y) * refPixelsPerUnit;

        Vector2D scale = entity->getComponent<TransformComponent>().scale;
        size_t regionIndex = entity->getComponent<SpriteComponent>().region;
        dst.w = textureAsset->regions[regionIndex].w * std::abs(scale.x) * pxlUnitScale;
        dst.h = textureAsset->regions[regionIndex].h * std::abs(scale.y) * pxlUnitScale;
        src = { textureAsset->regions[regionIndex].x, textureAsset->regions[regionIndex].y, 
        textureAsset->regions[regionIndex].w, textureAsset->regions[regionIndex].h };

        // View culling: skip sprites fully outside the logical viewport.
        if (dst.x + dst.w < 0.f || dst.y + dst.h < 0.f ||
            dst.x > static_cast<float>(logicalSize.x) || dst.y > static_cast<float>(logicalSize.y)) continue;

        TextureManager::Draw(textureAsset->texture, &src, &dst, 
            entity->getComponent<SpriteComponent>().spriteFlip, entity->getComponent<TransformComponent>().rotation);
        }
}

} // namespace PRL