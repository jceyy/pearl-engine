#include "ECS/SensorComponent.hpp"

namespace PRL {

size_t SensorComponent::instanceCount_ = 0;

SensorComponent::SensorComponent() : count_(0) {
    instanceCount_++;
}

SensorComponent::SensorComponent(const SensorComponent& other) : count_(other.count_) {
    for (int i = 0; i < count_; ++i)
        regions_[i] = other.regions_[i];
    instanceCount_++;
}

SensorComponent::~SensorComponent() {
    instanceCount_--;
}

int SensorComponent::add(const Collision::SensorRegion& region) {
    if (count_ >= MAX_REGIONS) return -1;
    regions_[count_] = region;
    return count_++;
}

void SensorComponent::remove(int index) {
    if (index < 0 || index >= count_) return;
    for (int i = index; i < count_ - 1; ++i)
        regions_[i] = regions_[i + 1];
    count_--;
}

void SensorComponent::setActive(int index, bool active) {
    if (index >= 0 && index < count_)
        regions_[index].active = active;
}

void SensorComponent::clearAll() {
    count_ = 0;
}

} // namespace PRL
