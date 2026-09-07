#include <iostream>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include "PRL.hpp"

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
    
    // Engine initialization 
    uint32_t renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    uint32_t windowFlags = 0;

    PRL::Core::init({SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600}, renderFlags, windowFlags, "Game engine test");
    
    // --- Fixed-timestep simulation, decoupled from rendering -----------------
    // update() runs at a constant FIXED_DT (deterministic physics); render()
    // runs once per loop iteration and is throttled by vsync (PRESENTVSYNC).
    // Classic accumulator pattern: https://gafferongames.com/post/fix_your_timestep/
    constexpr double FIXED_DT = 1.0 / 120.0;   // seconds per simulation step
    constexpr double MAX_FRAME_TIME = 0.25;    // clamp to avoid the spiral of death
    PRL::Core::setFixedDeltaTime(FIXED_DT);

    PRL::Game *game = nullptr;

    try {
        game = new PRL::Game();
        game->init();

        PRL::Core::updateCurrentTime();
        uint64_t previousUS = PRL::Core::getCurrentTimeUS();
        double accumulator = 0.0;

        while(PRL::Game::isRunning){
            PRL::Core::updateCurrentTime();
            const uint64_t nowUS = PRL::Core::getCurrentTimeUS();
            double frameTime = (nowUS - previousUS) * 1e-6;
            previousUS = nowUS;
            if (frameTime > MAX_FRAME_TIME) frameTime = MAX_FRAME_TIME;
            accumulator += frameTime;

            game->handleEvents();

            while (accumulator >= FIXED_DT) {
                game->update();
                accumulator -= FIXED_DT;
            }

            game->render();
        }
    } catch(std::exception& e){
        PRL::Logging::log("Exception thrown: " + std::string(e.what()), "main");
        std::cerr << "Exception thrown: " << e.what() << std::endl;
    }

    // Clean the game (Game::~Game calls clean()), then the engine + SDL
    delete game;
    PRL::Core::clean();

    // Wait for enter press to close console, to allow reading of logs in DEBUG mode
    #ifndef NDEBUG
    #if defined(_WIN32) || defined(_WIN64)
        system("pause");
    #endif // Windows specific code to pause the console
    #endif // NDEBUG
    return 0;
}
