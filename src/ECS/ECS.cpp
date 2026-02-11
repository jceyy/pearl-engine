#include "ECS/ECS.hpp"
#include "Systems/Systems.hpp"

Entity::Entity(EntityManager& entityManager) : isActive_(true),
entityManager_(entityManager) {}

void Entity::destroy() {
    isActive_ = false;
}

bool Entity::hasGroup(EntityGroup group) const {
    return groupBitSet_[group]; 
}

void Entity::addGroup(EntityGroup group){
    groupBitSet_[group] = true;
    entityManager_.addToGroup(this, group);
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
    SystemManager* sysManager = entityManager_.getSystemManager();
    if (sysManager != nullptr) {
        sysManager->entitySignatureChanged(this, componentSignature_);
    }
}


EntityManager::EntityManager(const SystemManager* systemManager) : systemManager_(systemManager) {
    if (systemManager_ == nullptr) {
        PRL::err("SystemManager pointer is null", "EntityManager::EntityManager");
    }
}

void EntityManager::update() {
    // Update systems, if an entity is added / destroyed or if it has a change in component signature, the system entityManager will update the list of entities of each system, so we don't need to check for that here

    // to be deprecated
    for (auto& e : entities_) e->update();
}

void EntityManager::draw() {
    // to be deprecated
    for (auto& e : entities_) e->draw();
}

void EntityManager::refresh() {
    // remove unactive entities from groups
    for(auto i(0u); i < maxGroups; ++i){
        auto& v(groupedEntities_[i]);
        v.erase(
            std::remove_if(std::begin(v), std::end(v), 
            [i](Entity* entity) {
                return !entity->isActive() || !entity->hasGroup(i);
            }), std::end(v));
    }

    // remove from managed system entity lists :
    // - if entitiy is destroyed, or 
    // - if entity doesn't match system signature anymore
    const ComponentBitSet nullSignature = 0u;
    for (size_t i = 0; i < SystemID::maxSystemID; ++i) {
        auto& v = entitiesPerSystem_[i];
        v.erase(
            std::remove_if(std::begin(v), std::end(v),
            [](Entity* entity) {
                return (
                    !entity->isActive() || (
                        entity->getComponentSignature() != nullSignature &&
                        entity->getComponentSignature() & systemManager_->getSignature(i) != systemManager_->getSignature(i)
                    )
                );
            }), std::end(v));
    }

    // remove unactive entities from main storage list
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
    // at this point, entity has no component
    return *e;
}

size_t EntityManager::entityCount() const {
    return entities_.size();
}
