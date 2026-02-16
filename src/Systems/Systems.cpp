#include <algorithm>
#include "Systems/Systems.hpp"
#include "ECS/ECS.hpp"
#include "AssetManager.hpp"

using namespace std;

// System implementation //
size_t System::instanceCount_ = 0;

System::System() : signature_(0u), entityManager_(nullptr) {
    instanceCount_++;
}

System::System(ComponentSignature signature) : 
signature_(signature) {
    instanceCount_++;
}

System::~System() {
    instanceCount_--;
}

// SystemManager implementation //
size_t SystemManager::instanceCount_ = 0;
SystemManager::SystemManager(EntityManager& entityManager, AssetManager& assetManager) : 
    systems_(vector<unique_ptr<System>>(0)),
    systemArray_(array<System*, ECS::maxSystems>{nullptr}),
    registeredSystems_(0u),
    entityManager_(entityManager),
    assetManager_(assetManager) {
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
    cout << "[DEBUG] SystemManager updating " << systems_.size() << " systems\n";
    for (size_t i = 0; i < systems_.size(); ++i) {
        systems_[i]->update();
    }
}

void SystemManager::draw() {
    cout << "[DEBUG] SystemManager drawing " << systems_.size() << " systems\n";
    for (size_t i = 0; i < systems_.size(); ++i) {
        systems_[i]->draw();
    }
}
