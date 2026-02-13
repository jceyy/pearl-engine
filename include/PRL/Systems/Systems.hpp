#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <vector>
#include <memory>
#include <array>
#include "Types.hpp"
#include "ECS/ECSBasics.hpp"
#include "Logging.hpp"

class EntityManager;


class System : public PRLObject {
public:
    System();
    System(EntityManager* entityManager, ComponentSignature signature = ComponentSignature(0u));
    System(const System& other) = delete;
    System(System&& other) noexcept = delete;
    System& operator=(const System& other) = delete;
    System& operator=(System&& other) noexcept = delete;
    virtual ~System() = 0;
    
    // void setEntityManager(EntityManager& entityManager, ComponentSignature signature);

    virtual void update() = 0;
    virtual void draw() = 0;

    inline static size_t getInstanceCount() noexcept { return instanceCount_; }
    inline ComponentSignature getSignature() const noexcept { return signature_; }

protected:
    ComponentSignature signature_; //!< Component signature of entities updated by this system
    EntityManager* entityManager_; //!< Access to entities matching this system

private:
    static size_t instanceCount_;

    friend class SystemManager;
};


class SystemManager : public PRLObject {
public:
    SystemManager() = delete;
    SystemManager(EntityManager& entityManager);
    SystemManager(const SystemManager& other) = delete;
    SystemManager(SystemManager&& other) noexcept = delete;
    SystemManager& operator=(const SystemManager& other) = delete;
    SystemManager& operator=(SystemManager&& other) noexcept = delete;
    ~SystemManager();

    template <typename T, typename... TArgs>
    T& registerSystem(TArgs&&... mArgs) { 
        std::size_t systemID = SystemID::getSystemTypeID<T>(); 
        if (systemArray_[systemID] != nullptr) {
            // System already registered, return existing
            return *static_cast<T*>(systemArray_[systemID]);
        }
        T* c(new T(std::forward<TArgs>(mArgs)...));
        c->entityManager_ = &entityManager_;
        std::unique_ptr<System> uPtr { c };
        systems_.emplace_back(std::move(uPtr)); 

        systemArray_[systemID] = c;
        registeredSystems_[systemID] = true;
        return *c;
    }

    inline bool isSystemRegistered(std::size_t systemID) const noexcept {
        return registeredSystems_[systemID];
    }

    template <typename T>
    inline bool isSystemRegistered() const {
        return registeredSystems_[SystemID::getSystemTypeID<T>()];
    }

    ComponentSignature getSignature(std::size_t systemID) const;

    void update();
    void draw();

    static inline size_t getInstanceCount() noexcept { return instanceCount_; }

private:
    std::vector<std::unique_ptr<System>> systems_;
    std::array<System*, ECS::maxSystems> systemArray_;
    std::bitset<ECS::maxSystems> registeredSystems_;
    EntityManager& entityManager_;

    static size_t instanceCount_;
};

#endif // SYSTEM_HPP