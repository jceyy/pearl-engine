#include "KeyboardController.hpp"

void KeyboardController::init() {
    entity->addComponent<TransformComponent>();
    transform_ = &entity->getComponent<TransformComponent>();
    sprite_ = &entity->getComponent<SpriteComponent>();
}

void KeyboardController::update() {
    if (Game::event.type == SDL_KEYDOWN){
        switch (Game::event.key.keysym.sym)
        {
        case SDLK_UP:
            transform_->velocity.y = -1;
            sprite_->play("Walk");
            break;

        case SDLK_DOWN:
            transform_->velocity.y = 1;
            sprite_->play("Walk");
            break;
        
        case SDLK_LEFT:
            transform_->velocity.x = -1;
            sprite_->play("Walk");
            sprite_->setFlip(SDL_FLIP_HORIZONTAL);
            break;
        
        case SDLK_RIGHT:
            transform_->velocity.x = 1;
            sprite_->setFlip(SDL_FLIP_NONE);
            sprite_->play("Walk");
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
        
        case SDLK_ESCAPE:
            Game::isRunning = false;
            break;
            
        default:
            break;
        }
        if (transform_->velocity.x == 0 and transform_->velocity.y == 0){
            sprite_->play("Idle");
        }
    }
    if (transform_->velocity.x < 0) transform_->velocity.x = -1;
    else if (transform_->velocity.x > 0) transform_->velocity.x = 1;
    if (transform_->velocity.y < 0) transform_->velocity.y = -1;
    else if (transform_->velocity.y > 0) transform_->velocity.y = 1;
    transform_->velocity.normalize();
}