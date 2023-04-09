#include "Game.hpp"
#include "TextureManager.hpp"
#include "GameObject.hpp"


GameObject* player;
GameObject* enemy;

Game::Game() {}


Game::~Game() {}


void Game::init(const std::string& title, int xpos, int ypos, int width, int height, bool fullscreen) {
    
    int windowFlags = 0;
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        std::cout << "SDL subsystems initialized" << std::endl;
        if (fullscreen){
            windowFlags = SDL_WINDOW_FULLSCREEN;
        }
        window_ = SDL_CreateWindow(title.c_str(), xpos, ypos, width, height, windowFlags);
        if (window_){
            std::cout << "Window created" << std::endl;
        }

        renderer_ = SDL_CreateRenderer(window_, -1, 0);
        if (renderer_){
            SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
            std::cout << "Renderer created" << std::endl;
        }

        isRunning_ = true;
        player = new GameObject("../assets/parachute.png", renderer_, 0, 0);
        enemy = new GameObject("../assets/camera.png", renderer_, 80, 80);
    }
    else{
        isRunning_ = false;
    }

}


void Game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type){
        case SDL_QUIT:
        isRunning_ = false;
        break;

        default:
        break;
    }
}

int cpt = 0;
void Game::render() {
    SDL_RenderClear(renderer_);
    SDL_Rect dst = {cpt, 0, 130, 160};
    player->render();
    enemy->render();
    SDL_RenderPresent(renderer_);
    cpt++;
}


void Game::update() {
    player->update();
    enemy->update();
}


void Game::clean() {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}

