#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Collision.hpp"

class Game{

public:
    Game();
    ~Game();

    void init(const std::string& title, int xpos, int ypos, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();
    
    static SDL_Renderer *renderer;
    static SDL_Event event;
    // static std::vector<ColliderComponent*> colliders;

    static bool isRunning;
    static SDL_FRect camera;

    enum groupLabels : std::size_t {
        groupMap,
        groupPlayers,
        groupEnemies,
        groupColliders
    };

private:
    SDL_Window *window_;
};

#endif // GAME_HPP