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

    inline bool isRunning() {return isRunning_;};
    
    static void addTile(float x, float y, int id);
    static SDL_Renderer *renderer;
    static SDL_Event event;
    static std::vector<ColliderComponent*> colliders;

private:
    bool isRunning_;
    SDL_Window *window_;
};

#endif // GAME_HPP