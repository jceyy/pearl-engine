#ifndef _GAME_HPP_INCLUDED
#define _GAME_HPP_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Types.hpp"
#include "Collision.hpp"
#include "AssetManager.hpp"
#include "Systems/Systems.hpp"

namespace PRL {

    class Game : public PRLObject {
    public:
        Game();
        Game(const Game& other) = delete;
        Game(Game&& other) noexcept = delete;
        Game& operator=(const Game& other) = delete;
        Game& operator=(Game&& other) noexcept = delete;
        ~Game();

        void init();

        void handleEvents();
        void update();
        void render();
        void clean();
        
        static SDL_Event event;

        static bool isRunning;
        static SDL_FRect camera;
        constexpr static int pixelsPerUnit = 1;

        enum groupLabels : std::size_t {
            groupMap,
            groupPlayers,
            groupEnemies,
            groupColliders,
            groupProjectiles
        };

        inline static size_t getInstanceCount() noexcept { return instanceCount_; }
        
    private:        
        static size_t instanceCount_;
    };
}

#endif // _GAME_HPP_INCLUDED