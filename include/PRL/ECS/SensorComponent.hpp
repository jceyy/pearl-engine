#ifndef PRL_SENSOR_COMPONENT_HPP_INCLUDED
#define PRL_SENSOR_COMPONENT_HPP_INCLUDED

#include "../Collision.hpp"
#include "ECS.hpp"
#include "../Types.hpp"

namespace PRL {

// Holds up to MAX_REGIONS sensor regions (hitboxes, triggers, sense areas) in local space.
// The CollisionSystem evaluates overlaps each frame and writes results to a separate event
// buffer — no callbacks are stored here.
class SensorComponent : public Component {
public:
    static constexpr int MAX_REGIONS = 8;

    SensorComponent();
    SensorComponent(const SensorComponent&);
    ~SensorComponent();

    // Appends a region. Returns its index, or -1 if the array is full.
    int  add(const Collision::SensorRegion& region);

    // Removes the region at index, shifting later entries down.
    void remove(int index);

    // Convenience: enable or disable a single region without removing it.
    void setActive(int index, bool active);

    // Removes all regions.
    void clearAll();

    int  count() const noexcept { return count_; }
    bool isFull() const noexcept { return count_ == MAX_REGIONS; }

    Collision::SensorRegion&       operator[](int i)       { return regions_[i]; }
    const Collision::SensorRegion& operator[](int i) const { return regions_[i]; }

    static inline size_t getInstanceCount() noexcept { return instanceCount_; }

private:
    Collision::SensorRegion regions_[MAX_REGIONS];
    int                     count_ = 0;

    static size_t instanceCount_;
};

} // namespace PRL

#endif // PRL_SENSOR_COMPONENT_HPP_INCLUDED
