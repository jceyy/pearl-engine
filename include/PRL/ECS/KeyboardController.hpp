#ifndef KEYBOARD_CONTROLLER_HPP
#define KEYBOARD_CONTROLLER_HPP

#include "../Game.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class SpriteComponent;

class KeyboardController : public Component{
public:
    void init() override;
    void update() override;

private:
    TransformComponent* transform_;
    SpriteComponent* sprite_;
};

#endif // KEYBOARD_CONTROLLER_HPP