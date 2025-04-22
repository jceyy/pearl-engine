#ifndef _GAME_HPP_INCLUDED
#define _GAME_HPP_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Collision.hpp"
#include "AssetManager.hpp"


class Game{

public:
    Game();
    ~Game();

    void init(const std::string& title, int xpos, int ypos, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();

    static Uint64 current_time_us();
    
    static SDL_Renderer* renderer;
    static SDL_Event event;
    static AssetManager* assetManager;

    static bool isRunning;
    static SDL_FRect camera;

    enum groupLabels : std::size_t {
        groupMap,
        groupPlayers,
        groupEnemies,
        groupColliders,
        groupProjectiles
    };

private:
    SDL_Window *window_;
    
    static Uint64 current_time_us_;
};

#endif // _GAME_HPP_INCLUDED