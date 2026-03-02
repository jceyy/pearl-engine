#include <iostream>
#include <thread>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include "Core.hpp"
#include "PRL/Game.hpp"

using namespace std;
using namespace PRL;

int main(int argc, char** argv) {
    #if defined(_WIN32) || defined(_WIN64)
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    #endif // Windows specific code to open a console window
    
    std::cout << "Argument number : " << argc << std::endl;
    for (int i(0); i < argc; ++i) std::cout << argv[i] << std::endl;
    std::cout << std::endl;
    
    // Engine initialization 
    uint32_t renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    uint32_t windowFlags = 0;
    PRL::Core::init({SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600}, renderFlags, windowFlags, "Game engine test");
    
    // Game initialization
    const uint64_t FPS = 60;
    const uint64_t FRAME_DELAY = 1000000UL / FPS;
    uint64_t frameStart(0), frameTime(0);
    Game *game = nullptr;

    try {
        game = new Game();
        game->init();
        
        while(Game::isRunning){
            frameStart = PRL::Core::getCurrentTimeUS();

            game->handleEvents();
            game->update();
            game->render();

            frameTime = PRL::Core::getCurrentTimeUS() - frameStart;
            if (FRAME_DELAY > frameTime){
                // auto timeStart = std::chrono::high_resolution_clock::now();
                std::this_thread::sleep_for(static_cast<std::chrono::microseconds>(FRAME_DELAY - frameTime));
                // auto sleptFor = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - timeStart).count();
                // // std::cout << "main() : Slept for " << sleptFor << " us; Planned for " 
                // //           << FRAME_DELAY - frameTime << " us. (rel. diff of "
                // //           << (sleptFor - (FRAME_DELAY - frameTime)) / static_cast<double>(FRAME_DELAY - frameTime) * 100 << "%, "
                // //           << (sleptFor - (FRAME_DELAY - frameTime)) << " us)" << std::endl;
            }
        }
    } catch(std::exception& e){
        PRL::Logging::log("Exception thrown: " + std::string(e.what()), "main");
        std::cerr << "Exception thrown: " << e.what() << std::endl;
    }

    // Clean the game
    game->clean();
    // Clean the engine + SDL
    PRL::Core::clean();

    // Wait for enter press to close console, to allow reading of logs in DEBUG mode
    #ifndef NDEBUG
    #if defined(_WIN32) || defined(_WIN64)
        system("pause");
    #endif // Windows specific code to pause the console
    #endif // NDEBUG
    return 0;
}
