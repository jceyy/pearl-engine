#include "ECS/ECS.hpp"
#include "Systems/Systems.hpp"
#include "Logging.hpp"
#include "ECS/SpriteComponent.hpp" // temporary for debug purposes

using namespace std;

size_t Entity::instanceCount_ = 0;
Entity::Entity() : isActive_(true), entityManager_(nullptr) {
    instanceCount_++;
}

Entity::Entity(EntityManager* manager) : isActive_(true),
entityManager_(manager) {
    assert(manager != nullptr);
    instanceCount_++;
}

Entity::~Entity() {
    instanceCount_--;
}

void Entity::destroy() {
    isActive_ = false;
}

bool Entity::hasGroup(EntityGroup group) const {
    return groupBitSet_[group]; 
}

void Entity::addGroup(EntityGroup group){
    groupBitSet_[group] = true;
    entityManager_->addToGroup(this, group);
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
    entityManager_->entitySignatureChanged(this, componentSignature_);
}

// EntityManager implementation //
size_t EntityManager::instanceCount_ = 0;
EntityManager::EntityManager() :
    entities_(vector<unique_ptr<Entity>>(0)), 
    groupedEntities_(array<vector<Entity*>, ECS::maxGroups>({vector<Entity*>()})),
    entitiesPerSystem_(array<vector<Entity*>, ECS::maxSystems>({vector<Entity*>()})),
    systemManager_(nullptr) {
    instanceCount_++;
}

EntityManager::~EntityManager() {
    instanceCount_--;
}

void EntityManager::setSystemManager(SystemManager* systemManager) {
    assert(systemManager != nullptr);
    systemManager_ = systemManager;
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
    for(auto i(0u); i < ECS::maxGroups; ++i){
        auto& v(groupedEntities_[i]);
        v.erase(
            std::remove_if(std::begin(v), std::end(v), 
            [i](Entity* entity) {
                return !entity->isActive() || !entity->hasGroup(i);
            }), std::end(v));
    }

    // remove inactive entities from system entity lists
    for (size_t i = 0; i < SystemID::maxSystemID; ++i) {
        auto& v = entitiesPerSystem_[i];
        v.erase(
            std::remove_if(std::begin(v), std::end(v),
            [](Entity* entity) {
                return !entity->isActive();
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
    Entity* e = new Entity(this);
    std::unique_ptr<Entity> uPtr{e};
    entities_.emplace_back(std::move(uPtr));
    // at this point, entity has no component
    return *e;
}

// SystemManager* EntityManager::getSystemManager() const {
//     return systemManager_;
// }

std::vector<Entity*>& EntityManager::getEntitiesForSystem(std::size_t systemID) {
    return entitiesPerSystem_[systemID];
}

void EntityManager::entitySignatureChanged(Entity* entity, ComponentSignature entitySignature) {
    // cout << "[DEBUG] EntityManager::entitySignatureChanged called" << endl;
    if (systemManager_ == nullptr) {
        PRL::Logging::log("System manager not set in EntityManager", "EntityManager::entitySignatureChanged()");
        return;
    }
    // cout << "[DEBUG] Entity " << entity << " signature changed. Matches RenderSystem ? " 
        //  << ComponentSignature::create<SpriteComponent>().matches(entitySignature) << endl;

    // Update entity presence in each system's entity list
    for (std::size_t systemID = 0; systemID < SystemID::maxSystemID; ++systemID) {
        ComponentSignature systemSignature = systemManager_->getSignature(systemID);
        // cout << "[DEBUG] Checking system " << systemID << " with signature " << systemSignature.bitset() << " against entity signature " << entitySignature.bitset() << endl;
        if (systemSignature.none()) { 
            continue;
        }
        
        auto& systemEntities = entitiesPerSystem_[systemID];
        
        // Check if entity matches system signature
        bool matches = systemSignature.matches(entitySignature);
        
        // Find if entity is already in the list
        auto it = std::find(systemEntities.begin(), systemEntities.end(), entity);
        bool inList = (it != systemEntities.end());
        
        if (matches && !inList) {
            // Entity now matches, add it
            systemEntities.push_back(entity);
        } else if (!matches && inList) {
            // Entity no longer matches, remove it
            systemEntities.erase(it);
        }
    }
}
