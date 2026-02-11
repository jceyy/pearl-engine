#include "ECS/ECS.hpp"
#include "Systems/Systems.hpp"

Entity::Entity(EntityManager& manager) : isActive_(true),
manager_(manager) {}

void Entity::destroy() {
    isActive_ = false;
    SystemManager* sysManager = manager_.getSystemManager();
    if (sysManager != nullptr) {
        sysManager->entityDestroyed(this);
    }
}

bool Entity::isActive() const {
    return isActive_;
}

bool Entity::hasGroup(EntityGroup group) const {
    return groupBitSet_[group]; 
}

void Entity::addGroup(EntityGroup group){
    groupBitSet_[group] = true;
    manager_.addToGroup(this, group);
}

void Entity::delGroup(EntityGroup group){
    groupBitSet_[group] = false;
}

void Entity::update() {
    for (auto& c : components_) c->update();
}

void Entity::draw() {
    for (auto& c : components_) c->draw();
}

void Entity::notifySignatureChange_() {
    SystemManager* sysManager = manager_.getSystemManager();
    if (sysManager != nullptr) {
        sysManager->entitySignatureChanged(this, componentBitSet_);
    }
}


EntityManager::EntityManager() : systemManager_(nullptr) {}

void EntityManager::update() {
    for (auto& e : entities_) e->update();
}

void EntityManager::draw() {
    for (auto& e : entities_) e->draw();
}

void EntityManager::refresh() {

    for(auto i(0u); i < maxGroups; ++i){
        auto& v(groupedEntities_[i]);
        v.erase(
            std::remove_if(std::begin(v), std::end(v), 
            [i](Entity* entity) {
                return !entity->isActive() || !entity->hasGroup(i);
            }), std::end(v));
    }

    entities_.erase(std::remove_if(std::begin(entities_), std::end(entities_),
    [](const std::unique_ptr<Entity> &mEntity) {
        return !mEntity->isActive();
    }), std::end(entities_));
}

void EntityManager::addToGroup(Entity* entity, EntityGroup group) {
    groupedEntities_[group].emplace_back(entity);
}

std::vector<Entity*>& EntityManager::getGroup(EntityGroup group) {
    return groupedEntities_[group];
}

Entity& EntityManager::addEntity() {
    Entity* e = new Entity(*this);
    std::unique_ptr<Entity> uPtr{e};
    entities_.emplace_back(std::move(uPtr));
    return *e;
}

size_t EntityManager::entityCount() const {
    return entities_.size();
}
