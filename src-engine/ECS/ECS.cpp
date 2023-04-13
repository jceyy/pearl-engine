#include "ECS.hpp"

Entity::Entity() : isActive_(true) {}


void Entity::update() {
    for (auto& c : components_) c->update();
}

void Entity::draw() {
    for (auto& c : components_) c->draw();
}


void EntityManager::update() {
    for (auto& e : entities_) e->update();
}

void EntityManager::draw() {
    for (auto& e : entities_) e->draw();
}

void EntityManager::refresh() {
    entities_.erase(std::remove_if(std::begin(entities_), std::end(entities_),
    [](const std::unique_ptr<Entity> &mEntity) {
        return !mEntity->isActive();
    }), std::end(entities_));
}


Entity& EntityManager::addEntity() {
    Entity* e = new Entity();
    std::unique_ptr<Entity> uPtr{e};
    entities_.emplace_back(std::move(uPtr));
    return *e;
}

size_t EntityManager::entityCount() const {
    return entities_.size();
}
