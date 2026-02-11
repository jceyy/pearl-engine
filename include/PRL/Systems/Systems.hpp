#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <vector>
#include <unordered_map>
#include <memory>
#include "Logging.hpp"
#include "../ECS/ECS.hpp"

class SystemID : public PRLObject {
public:
    template<typename T>
    static inline std::size_t value() {
        static_assert(std::is_base_of<System, T>::value, "T must inherit from System");
        static const std::size_t id = idCounter_++;
        static_assert(id < maxSystemID, "Exceeded maximum number of systems");
        return id;
    }
    constexpr static std::size_t maxSystemID = 32;

private:
    static std::size_t idCounter_;
};


class System : public PRLObject {
public:
    System(EntityManager* entityManager); // add instance counter in this class
    System(const System& other) = delete;
    System(System&& other) noexcept = delete;
    System& operator=(const System& other) = delete;
    System& operator=(System&& other) noexcept = delete;
    virtual ~System() = 0;
    
    virtual void update() = 0;
    virtual void draw() = 0;

    std::vector<Entity*> entities; // Entities matching this system's signature
    inline static size_t getInstanceCount() noexcept { return instanceCount_; }

protected:
    ComponentBitSet signature_; //!< Component signature of entities updated by this system

private:
    EntityManager* entityManager_;
    static size_t instanceCount_;

};


class SystemManager : public PRLObject {
public:
    template <typename T, typename... TArgs>
    std::shared_ptr<T> registerSystem(TArgs&&... mArgs) {
        if (systems_[SystemID::value<T>()] != nullptr) {
            // System already registered
            return std::static_pointer_cast<T>(systems_[SystemID::value<T>()]);
        }

        std::shared_ptr<T> system = std::make_shared<T>(std::forward<TArgs>(mArgs)...);
        systems_[SystemID::value<T>()] = system;
        // signatures_[SystemID::value<T>()] = system->signature_;
        return system;
    }

    ComponentBitSet getSignature(SystemID systemID) const {
        return signatures_[systemID.value()];
    }

    void entityDestroyed(Entity* entity); // to be deprecated, dealt with in EntityManager
    void entitySignatureChanged(Entity* entity, ComponentBitSet entitySignature);

    inline ComponentBitSet getSignature(SystemID systemID) const {
        return signatures_[systemID.value()];
    } 

    void update();
    void draw();

private:
    // std::array<ComponentBitSet, SystemID::maxSystemID> signatures_;
    std::array<std::shared_ptr<System>, SystemID::maxSystemID> systems_;
};

#endif // SYSTEM_HPP