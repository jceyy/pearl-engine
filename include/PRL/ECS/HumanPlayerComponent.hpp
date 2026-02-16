#ifndef _HUMAN_PLAYER_COMPONENT_HPP_INCLUDED
#define _HUMAN_PLAYER_COMPONENT_HPP_INCLUDED

#include "ECS/ECS.hpp"


class HumanPlayerComponent : public Component {
public:
    HumanPlayerComponent() : playerID(0) {
        instanceCount_++;
    };
    HumanPlayerComponent(const HumanPlayerComponent& other) : playerID(other.playerID) {
        instanceCount_++;
    };
    ~HumanPlayerComponent() {
        instanceCount_--;
    }

    void init() override {}; // to be removed !
    void update() override {}; // to be removed !
    void draw() override {}; // to be removed !

    uint8_t playerID;

    static inline size_t getInstanceCount() noexcept { return instanceCount_; }
    
private:
    static size_t instanceCount_;
};

#endif // _ANIMATION_COMPONENT_HPP_INCLUDED