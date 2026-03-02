#include "AssetManager.hpp"
#include "Systems/AnimationSystem.hpp"


using namespace std;

size_t AnimationSystem::instanceCount_ = 0;

AnimationSystem::AnimationSystem(ComponentSignature signature) :
System(signature), deltaTimeUS_(1e6 / 30) {
    systemName_ = "AnimationSystem";
    instanceCount_++;
}

AnimationSystem::~AnimationSystem() {
    instanceCount_--;
}

void AnimationSystem::update() {
    // Get entities from EntityManager that match this system's signature
    auto& entities = entityManager_->getEntitiesForSystem(SystemID::getSystemTypeID<AnimationSystem>());

    for (size_t i = 0; i < entities.size(); ++i) {
        Entity* entity = entities[i];
        AnimationComponent& animComponent = entity->getComponent<AnimationComponent>();
        SpriteComponent& spriteComponent = entity->getComponent<SpriteComponent>();
        if (!spriteComponent.visible || !animComponent.playing) continue;

        const AnimationHandle& animHandle = animComponent.animHandle;
        const AnimationAsset* animAsset = assetManager_->getAnimationAsset(animHandle);
        if (!animAsset) {
            PRL::Logging::err("Invalid animation handle in AnimationSystem update");
            continue;
        }
        animComponent.runTime += deltaTimeUS_;
        uint64_t frameDurationUS = static_cast<uint64_t>(1e6 / animAsset->fps);

        while (animComponent.runTime >= frameDurationUS) {
            animComponent.runTime -= frameDurationUS;
            animComponent.currentFrame++;

            if (animComponent.currentFrame >= animAsset->frameRegions.size()) {
                if (animAsset->looping) {
                    animComponent.currentFrame = 0;
                } else {
                    animComponent.currentFrame = animAsset->frameRegions.size() - 1;
                    animComponent.playing = false;
                }
            }
        }
        spriteComponent.region = assetManager_->getAnimationAsset(animHandle)->frameRegions[animComponent.currentFrame];
    }
}