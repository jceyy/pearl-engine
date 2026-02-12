#include "Systems/Systems.hpp"
#include "ECS/ECS.hpp"
#include <algorithm>

using namespace std;

// System implementation //
size_t System::instanceCount_ = 0;

System::System() : signature_(0u), entityManager_(nullptr) {
    instanceCount_++;
}

System::System(EntityManager* entityManager, ComponentSignature signature) : 
signature_(signature), entityManager_(entityManager) {
    instanceCount_++;
}

System::~System() {
    instanceCount_--;
}

// SystemManager implementation //
size_t SystemManager::instanceCount_ = 0;
SystemManager::SystemManager(EntityManager& entityManager) : 
    systems_(vector<unique_ptr<System>>(0)),
    systemArray_(array<System*, maxSystemID>{nullptr}),
    registeredSystems_(0u),
    entityManager_(entityManager) {
    instanceCount_++;
}

SystemManager::~SystemManager() {
    instanceCount_--;
}

ComponentSignature SystemManager::getSignature(size_t systemID) const {
    if (registeredSystems_[systemID]) {
        return systemArray_[systemID]->getSignature();
    } else {
        return ComponentSignature(0); // return empty signature if system is not registered
    }
}

void SystemManager::update() {
    for (size_t i = 0; i < maxSystemID; ++i) {
        if (registeredSystems_[i]) {
            systemArray_[i]->update();
        }
    }
}

void SystemManager::draw() {
    for (size_t i = 0; i < maxSystemID; ++i) {
        if (registeredSystems_[i]) {
            systemArray_[i]->draw();
        }
    }
}
