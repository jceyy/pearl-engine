#ifndef ENTITY_COMPONENT_SYSTEM_HPP
#define ENTITY_COMPONENT_SYSTEM_HPP

#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <bitset>
#include <algorithm>
#include <assert.h>
#include "Types.hpp"
#include "ECSBasics.hpp"

class EntityManager;
class SystemManager;
class Entity;

class Component : public PRLObject {
public:
    // To implement :
    Component() = default;
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    Component(Component&&) = delete;
    Component& operator=(Component&&) = delete;
    virtual ~Component() {};

    
    virtual void init() {};
    virtual void update() {}; // to be deprecated
    virtual void draw() {}; // to be deprecated
    
    Entity* entity; // not protected?
};


class Entity : public PRLObject {
public:
    Entity();
    Entity(EntityManager* manager);
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity(Entity&&) = delete;
    Entity& operator=(Entity&&) = delete;
    ~Entity();
    
    void update(); // to be deprecated
    void draw(); // to be deprecated
    void destroy();
    inline bool isActive() const noexcept { return isActive_; }
    bool hasGroup(EntityGroup group) const;
    void addGroup(EntityGroup group);
    void delGroup(EntityGroup group);

    inline ComponentSignature getComponentSignature() const {
        return componentSignature_;
    }

    template <typename T> inline bool hasComponent() const{
        return componentSignature_[static_cast<size_t>(ComponentID::getComponentTypeID<T>())];
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs){
        if (componentSignature_[static_cast<size_t>(ComponentID::getComponentTypeID<T>())]){
            // Component already added, returning it
            return *static_cast<T*>(componentArray_[static_cast<size_t>(ComponentID::getComponentTypeID<T>())]);
        }

        T* c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;
        std::unique_ptr<Component> uPtr { c };
        components_.emplace_back(std::move(uPtr));

        componentArray_[static_cast<size_t>(ComponentID::getComponentTypeID<T>())] = c;
        componentSignature_[static_cast<size_t>(ComponentID::getComponentTypeID<T>())] = true;

        c->init();
        notifySignatureChange_();
        return *c;
    }

    template <typename T> inline T& getComponent() const{
        auto ptr(componentArray_[static_cast<size_t>(ComponentID::getComponentTypeID<T>())]);
        return *static_cast<T*>(ptr);
    }

    static inline size_t getInstanceCount() noexcept { return instanceCount_; }

private:
    bool isActive_ = false;
    std::vector<std::unique_ptr<Component>> components_;
    std::array<Component*, ECS::maxComponents> componentArray_;
    ComponentSignature componentSignature_;
    EntityGroupBitSet groupBitSet_;
    EntityManager* entityManager_;

    static size_t instanceCount_;

    void notifySignatureChange_();
};

class EntityManager : public PRLObject {
public:
    EntityManager();
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;
    EntityManager(EntityManager&&) = delete;
    EntityManager& operator=(EntityManager&&) = delete;
    ~EntityManager();

    Entity& addEntity();
    void setSystemManager(SystemManager* systemManager);
    void update();
    void draw();
    void refresh();
    void addToGroup(Entity* entity, EntityGroup group);

    std::vector<Entity*>& getGroup(EntityGroup group);
    std::vector<Entity*>& getEntitiesForSystem(std::size_t systemID);
    
    void entitySignatureChanged(Entity* entity, ComponentSignature signature);
    // SystemManager* getSystemManager() const;
    inline size_t entityCount() const noexcept { return entities_.size(); }

    static inline size_t getInstanceCount() noexcept { return instanceCount_; }

private:
    std::vector<std::unique_ptr<Entity>> entities_;
    std::array<std::vector<Entity*>, ECS::maxGroups> groupedEntities_;
    std::array<std::vector<Entity*>, ECS::maxSystems> entitiesPerSystem_;
    SystemManager* systemManager_;

    static size_t instanceCount_;
};




// Rules for incompatible components :
// - KeyboardComponent and ControllerComponent ?
// - ...

#endif // ENTITY_COMPONENT_SYSTEM_HPP
