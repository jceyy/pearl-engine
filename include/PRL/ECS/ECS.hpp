#ifndef ENTITY_COMPONENT_SYSTEM_HPP
#define ENTITY_COMPONENT_SYSTEM_HPP

#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <bitset>
#include <algorithm>
#include <unordered_map>
#include "../PhysicsManager.hpp"

class Component;
class Entity;
class EntityManager;
class SystemManager;

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentID = std::size_t;
using EntityGroup = std::size_t;
using ComponentBitSet = std::bitset<maxComponents>;
using EntityGroupBitSet = std::bitset<maxGroups>;



inline ComponentID getNewComponentTypeID(){
    static ComponentID lastID = 0u;
    return lastID++;
}

template<typename T> inline ComponentID getComponentTypeID() noexcept{
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}

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
    Entity(EntityManager& manager);
    // To be implemented :
    ~Entity() = default;
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;
    Entity(Entity&&) = delete;
    Entity& operator=(Entity&&) = delete;

    void update(); // to be deprecated
    void draw(); // to be deprecated
    void destroy();
    inline bool isActive() const noexcept { return isActive_; }
    bool hasGroup(EntityGroup group) const;
    void addGroup(EntityGroup group);
    void delGroup(EntityGroup group);

    inline ComponentBitSet getComponentSignature() const {
        return componentSignature_;
    }

    template <typename T> inline bool hasComponent() const{
        return componentSignature_[getComponentTypeID<T>()];
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs){
        if (componentSignature_[getComponentTypeID<T>()]){
            // std::cout << "Component already added, returning it\n";
            return *static_cast<T*>(componentArray_[getComponentTypeID<T>()]);
        }

        T* c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;
        std::unique_ptr<Component> uPtr { c };
        components_.emplace_back(std::move(uPtr));

        componentArray_[getComponentTypeID<T>()] = c;
        componentSignature_[getComponentTypeID<T>()] = true;

        c->init();
        notifySignatureChange_();
        return *c;
    }

    template <typename T> inline T& getComponent() const{
        auto ptr(componentArray_[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }

private:
    bool isActive_ = false;
    std::vector<std::unique_ptr<Component>> components_;

    std::array<Component*, maxComponents> componentArray_;
    ComponentBitSet componentSignature_;
    EntityGroupBitSet groupBitSet_;
    EntityManager& manager_;

    void notifySignatureChange_();
};

class EntityManager : public PRLObject {
public:
    EntityManager(const SystemManager* systemManager);
    // To be implemented :
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;
    EntityManager(EntityManager&&) = delete;
    EntityManager& operator=(EntityManager&&) = delete;
    ~EntityManager() = default;

    Entity& addEntity();
    void destroyedEntity(Entity* entity);
    
    void update();
    void draw();
    void refresh();
    void addToGroup(Entity* entity, EntityGroup group);

    std::vector<Entity*>& getGroup(EntityGroup group);

    size_t entityCount() const;

private:

    std::vector<std::unique_ptr<Entity>> entities_;
    std::array<std::vector<Entity*>, maxGroups> groupedEntities_;
    SystemManager* systemManager_;
    std::array<std::vector<Entity*>, SystemID::maxSystemID> entitiesPerSystem_;
};

// Base case for recursion
template <typename T>
void addToSignature(ComponentBitSet& signature) {
    signature.set(getComponentTypeID<T>());
}

// Recursive case
template <typename T, typename... Rest>
void addToSignature(ComponentBitSet& signature) {
    signature.set(getComponentTypeID<T>());
    addToSignature<Rest...>(signature);
}

// Main function that creates the signature
template <typename... ComponentTypes>
ComponentBitSet createSignature() {
    ComponentBitSet signature;
    addToSignature<ComponentTypes...>(signature);
    return signature;
}

// Specialization for zero components
template <>
inline ComponentBitSet createSignature<>() {
    return ComponentBitSet();
}


// Rules for incompatible components :
// - KeyboardComponent and ControllerComponent ?
// - ...

#endif // ENTITY_COMPONENT_SYSTEM_HPP
