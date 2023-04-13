#ifndef KEYBOARD_CONTROLLER_HPP
#define KEYBOARD_CONTROLLER_HPP

#include "../Game.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class KeyboardController : public Component{
public:
    void init() override;
    void update() override;

private:
    TransformComponent* transform_;
};

#endif // KEYBOARD_CONTROLLER_HPP