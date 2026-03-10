#ifndef _TILE_COMPONENT_HPP_INCLUDED
#define _TILE_COMPONENT_HPP_INCLUDED

#include <SDL2/SDL.h>
#include "TileMap.hpp"
#include "ECS.hpp"
#include "../Types.hpp"

namespace PRL {

    class TileMapComponent : public Component {
    public:
        TileMapComponent() {}
        ~TileMapComponent() {} 

        TileMap map;

        static inline size_t getInstanceCount() noexcept { return instanceCount_; }

    private:
        static size_t instanceCount_;
    };

} // namespace PRL

#endif // _TILE_COMPONENT_HPP_INCLUDED