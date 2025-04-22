#include <iostream>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include "../include/PRL/Game.hpp"
using namespace std;

int main(int argc, char** argv) {
    #if defined(_WIN32) || defined(_WIN64)
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    #endif // Windows specific code to open a console window
    
    std::cout << "Argument number : " << argc << std::endl;
    for (int i(0); i < argc; ++i) std::cout << argv[i] << std::endl;
    std::cout << std::endl;
    
    const int FPS = 60;
    const int FRAME_DELAY = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    Game *game = new Game();
    game->init("Game engine test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

try{
    while(Game::isRunning){
        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime){
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
    game->clean();
}
catch(std::string e){
    std::cerr << "Exception thrown: " << e << std::endl;
}
    return 0;
}
