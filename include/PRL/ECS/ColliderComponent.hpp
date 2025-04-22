#ifndef _COLLIDER_COMPONENT_HPP_INCLUDED
#define _COLLIDER_COMPONENT_HPP_INCLUDED

#include <SDL2/SDL.h>
#include <string>
#include "../Types.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include "Collision.hpp"

// enum class ColliderType {
//     Player, Enemy, Terrain, Projectile, Tile, Button, Label
// };

// To do : Also FrameColliderComponent, which is a collider that is not updated every frame


class ColliderComponent : public Component {
public:
    // ColliderComponent();
    ColliderComponent(const std::string& tag, const ColliderShape& shape, bool isTrigger = false);
    ~ColliderComponent();

    void init() override;
    void update() override;
    void draw() override;

    inline const std::string&  getTag() const noexcept { return tag_; }
    inline bool isTrigger() const noexcept { return isTrigger_; }

private:
    bool isTrigger_;
    std::string tag_;
    ColliderShape* shape_;
    TransformComponent* transform_;
};

// To do : implement this class
class ColliderDebugComponent : public Component {
public:
    ColliderDebugComponent() {};
    ~ColliderDebugComponent() {};

    void init() override {};
    void update() override {};
    void draw() override {};

private:
    ColliderComponent* collider_;
    SDL_FRect dstRect_;
    SDL_Texture* texture_;
};

#endif // _COLLIDER_COMPONENT_HPP_INCLUDED