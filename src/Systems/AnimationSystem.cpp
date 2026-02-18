#include "AssetManager.hpp"
#include "Systems/AnimationSystem.hpp"

size_t AnimationSystem::instanceCount_ = 0;

AnimationSystem::AnimationSystem(ComponentSignature signature) :
System(signature), deltaTime_(1.0f) {
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

        const AnimationClip& clip = assetManager_->getAnimation(animComponent.animClipID);

        animComponent.runTime += deltaTime_;
        float frameDuration = 1.0f / clip.fps;

        while (animComponent.runTime >= frameDuration) {
            animComponent.runTime -= frameDuration;
            animComponent.currentFrame++;

            if (animComponent.currentFrame >= clip.frames.size()) {
                if (clip.looping)
                    animComponent.currentFrame = 0;
                else {
                    animComponent.currentFrame = clip.frames.size() - 1;
                    animComponent.playing = false;
                }
            }
        }

        spriteComponent.region = clip.frames[animComponent.currentFrame];
    }
}