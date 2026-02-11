#include "Systems/Systems.hpp"
#include <algorithm>

std::size_t SystemID::idCounter_ = 0;

// System implementation //
size_t System::instanceCount_ = 0;
System::System() : signature_(0u), systemManager_(nullptr) {
    instanceCount_++;
}

System::~System() {
    instanceCount_--;
}


// SystemManager implementation //
void SystemManager::entitySignatureChanged(Entity* entity, ComponentBitSet entitySignature) {
    for (auto it = systems_.begin(); it != systems_.end(); ++it) {
        
        const char* typeName = it->first;
        std::shared_ptr<System> system = it->second;
        ComponentBitSet systemSignature = signatures_[typeName];
        
        // Check if entity matches system signature
        ComponentBitSet intersection = entitySignature & systemSignature;
        if (intersection == systemSignature) {
            // Entity has all required components
            // Check if entity already exists in the system
            auto findIt = std::find(system->entities.begin(), system->entities.end(), entity);
            
            if (findIt == system->entities.end()) {
                system->entities.push_back(entity);
            }
        } else {
            // Entity doesn't match anymore, remove if present
            system->entities.erase(
                std::remove(system->entities.begin(), system->entities.end(), entity),
                system->entities.end()
            );
        }
    }
}

void SystemManager::entityDestroyed(Entity* entity) {
    for (auto it = systems_.begin(); it != systems_.end(); ++it) {
        
        std::shared_ptr<System> system = it->second;
        system->entities.erase(
            std::remove(system->entities.begin(), system->entities.end(), entity),
            system->entities.end()
        );
    }
}

void SystemManager::update() {
    for (auto it = systems_.begin(); it != systems_.end(); ++it) {
        it->second->update();
    }
}

void SystemManager::draw() {
    for (auto it = systems_.begin(); it != systems_.end(); ++it) {
        it->second->draw();
    }
}
