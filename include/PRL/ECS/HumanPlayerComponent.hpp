#ifndef _HUMAN_PLAYER_COMPONENT_HPP_INCLUDED
#define _HUMAN_PLAYER_COMPONENT_HPP_INCLUDED

#include "ECS/ECS.hpp"

namespace PRL {

using PlayerIDType = uint8_t;

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

    PlayerIDType playerID;

    static inline size_t getInstanceCount() noexcept { return instanceCount_; }
    
private:
    static size_t instanceCount_;
};

} // namespace PRL

#endif // _HUMAN_PLAYER_COMPONENT_HPP_INCLUDED