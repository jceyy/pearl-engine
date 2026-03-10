#ifndef TILE_MAP_SYSTEM_HPP
#define TILE_MAP_SYSTEM_HPP

#include "Systems.hpp"
#include "ECS/TileMapComponent.hpp"

namespace PRL {

class TileMapSystem : public System {
public:
    TileMapSystem(ComponentSignature signature = 
        ComponentSignature::create<TileMapComponent>());
    ~TileMapSystem();
    
    void update() override;
    void draw() override;

    static inline size_t getInstanceCount() noexcept { return instanceCount_; }

private:
    static size_t instanceCount_;
};

} // namespace PRL

#endif // TILE_MAP_SYSTEM_HPP