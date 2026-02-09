#ifndef _ENTITY_COMPONENT_SYSTEM_HPP
#define _ENTITY_COMPONENT_SYSTEM_HPP

#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <bitset>
#include <algorithm>
#include "../PhysicsManager.hpp"

class Component;
class Entity;
class EntityManager;

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentID = std::size_t;
using Group = std::size_t;
using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;



inline ComponentID getNewComponentTypeID(){
    static ComponentID lastID = 0u;
    return lastID++;
}

template<typename T> inline ComponentID getComponentTypeID() noexcept{
    static ComponentID typeID = getNewComponentTypeID();
    return typeID;
}

class Component{
public:
    Entity* entity;

    virtual void init() {};
    virtual void update() {};
    virtual void draw() {};

    virtual ~Component() {};
};


class Entity {
public:
    Entity(EntityManager& manager);
    void update();
    void draw();
    void destroy();
    bool isActive() const;
    bool hasGroup(Group group) const;
    void addGroup(Group group);
    void delGroup(Group group);

    template <typename T> bool hasComponent() const{
        return componentBitSet_[getComponentTypeID<T>()];
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs){
        if (componentBitSet_[getComponentTypeID<T>()]){
            // std::cout << "Component already added, returning it\n";
            return *static_cast<T*>(componentArray_[getComponentTypeID<T>()]);
        }

        T* c(new T(std::forward<TArgs>(mArgs)...));
        c->entity = this;
        std::unique_ptr<Component> uPtr { c };
        components_.emplace_back(std::move(uPtr));

        componentArray_[getComponentTypeID<T>()] = c;
        componentBitSet_[getComponentTypeID<T>()] = true;

        c->init();
        return *c;
    }

    template <typename T> T& getComponent() const{
        auto ptr(componentArray_[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }

private:
    bool isActive_ = false;
    std::vector<std::unique_ptr<Component>> components_;

    ComponentArray componentArray_;
    ComponentBitSet componentBitSet_;
    GroupBitSet groupBitSet_;
    EntityManager& manager_;

};

class EntityManager{
public:
    void update();
    void draw();
    void refresh();
    void addToGroup(Entity* entity, Group group);

    std::vector<Entity*>& getGroup(Group group);

    size_t entityCount() const;

    Entity& addEntity();

private:
    void checkCollisions_();

    std::vector<std::unique_ptr<Entity>> entities_;
    std::array<std::vector<Entity*>, maxGroups> groupedEntities_;

};

class System {
public:
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual ~System() = default;
};

#endif // _ENTITY_COMPONENT_SYSTEM_HPP
