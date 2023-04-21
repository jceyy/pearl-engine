#include "TileComponent.hpp"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"

TileComponent::TileComponent() :
transform_(nullptr), sprite_(nullptr), tileRect_({0, 0, 0, 0}), tileID_(0),
fileName_("") {
}


TileComponent::TileComponent(PosType x, PosType y, int w, int h, int id) :
transform_(nullptr), sprite_(nullptr) {
    tileRect_.x = x;
    tileRect_.y = y;
    tileRect_.w = w;
    tileRect_.h = h;
    tileID_ = id;

    switch (tileID_){    
    case 0:
        fileName_ = "assets/dirt.png";
        break;

    case 1:
        fileName_ = "assets/grass.png";
        break;

    case 2:
        fileName_ = "assets/water.png";
        break;

    default:
        std::cout << "Unknown tile type" << std::endl;
        break;
    }
}

void TileComponent::init(){
    entity->addComponent<TransformComponent>(tileRect_.x, tileRect_.y, 1);
    entity->addComponent<SpriteComponent>();

    transform_ = &entity->getComponent<TransformComponent>();
    sprite_ = &entity->getComponent<SpriteComponent>();
    sprite_->setTexture(fileName_, tileRect_.w, tileRect_.h);
}