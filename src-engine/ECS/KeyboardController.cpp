#include "KeyboardController.hpp"

void KeyboardController::init() {
    if (!entity->hasComponent<TransformComponent>()){
        entity->addComponent<TransformComponent>();
    }
    transform_ = &entity->getComponent<TransformComponent>();
}

void KeyboardController::update() {
    if (Game::event.type == SDL_KEYDOWN){
        switch (Game::event.key.keysym.sym)
        {
        case SDLK_UP:
            transform_->velocity.y = -1;
            break;

        case SDLK_DOWN:
            transform_->velocity.y = 1;
            break;
        
        case SDLK_LEFT:
            transform_->velocity.x = -1;
            break;
        
        case SDLK_RIGHT:
            transform_->velocity.x = 1;
            break;
        
        default:
            break;
        }
    }

    if (Game::event.type == SDL_KEYUP){
        switch (Game::event.key.keysym.sym)
        {
        case SDLK_UP:
            transform_->velocity.y = 0;
            break;

        case SDLK_DOWN:
            transform_->velocity.y = 0;
            break;
        
        case SDLK_LEFT:
            transform_->velocity.x = 0;
            break;

        case SDLK_RIGHT:
            transform_->velocity.x = 0;
            break;
        
        default:
            break;
        }
    }
    if (transform_->velocity.x < 0) transform_->velocity.x = -1;
    else if (transform_->velocity.x > 0) transform_->velocity.x = 1;
    if (transform_->velocity.y < 0) transform_->velocity.y = -1;
    else if (transform_->velocity.y > 0) transform_->velocity.y = 1;
    transform_->velocity.normalize();
}