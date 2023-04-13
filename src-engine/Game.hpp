#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
    static SDL_Renderer *renderer;
    static SDL_Event event;

private:
    bool isRunning_;
    SDL_Window *window_;
};

#endif // GAME_HPP