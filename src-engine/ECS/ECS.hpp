#ifndef ENTITY_COMPONENT_SYSTEM_HPP
#define ENTITY_COMPONENT_SYSTEM_HPP

#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <bitset>
#include <algorithm>

class Component;
class Entity;

constexpr std::size_t maxComponents = 32;
using ComponentID = std::size_t;
using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;


inline ComponentID getComponentTypeID(){
    static ComponentID lastID = 0;
    return lastID++;
}

template<typename T> inline ComponentID getComponentTypeID() noexcept{
    static ComponentID typeID = getComponentTypeID();
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
    Entity();
    void update();
    void draw();
    void destroy() {isActive_ = false;};
    bool isActive() const {return isActive_;};

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

};

class EntityManager{
public:
    void update();
    void draw();
    void refresh();
    size_t entityCount() const;

    Entity& addEntity();

private:
    std::vector<std::unique_ptr<Entity>> entities_;

};

#endif // ENTITY_COMPONENT_SYSTEM_HPP