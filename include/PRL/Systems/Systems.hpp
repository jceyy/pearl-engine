#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <vector>
#include <unordered_map>
#include <memory>
#include "../ECS/ECS.hpp"


class System {
public:
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual ~System() {}

    std::vector<Entity*> entities; // Entities matching this system's signature

protected:
    ComponentBitSet signature_; //!< Signature of components required by this system
};


class SystemManager {
public:
    template <typename T, typename... TArgs>
    std::shared_ptr<T> registerSystem(TArgs&&... mArgs) {
        const char* typeName = typeid(T).name();
        
        std::unordered_map<const char*, std::shared_ptr<System> >::iterator it = systems_.find(typeName);
        if (it != systems_.end()) {
            // System already registered
            return std::static_pointer_cast<T>(it->second);
        }

        std::shared_ptr<T> system = std::make_shared<T>(std::forward<TArgs>(mArgs)...);
        systems_[typeName] = system;
        return system;
    }

    template <typename T>
    void setSignature(ComponentBitSet signature) {
        const char* typeName = typeid(T).name();
        signatures_[typeName] = signature;
    }

    void entityDestroyed(Entity* entity);
    void entitySignatureChanged(Entity* entity, ComponentBitSet entitySignature);

    void update();
    void draw();

private:
    std::unordered_map<const char*, ComponentBitSet> signatures_;
    std::unordered_map<const char*, std::shared_ptr<System> > systems_; // Note: space before > for C++11
};

#endif // SYSTEM_HPP