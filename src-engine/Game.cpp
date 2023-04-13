#include "Game.hpp"
#include "TextureManager.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"
#include "Types.hpp"
#include "Collision.hpp"

EntityManager manager;
SDL_Event Game::event;
auto& wall(manager.addEntity());
auto& player(manager.addEntity());


SDL_Renderer* Game::renderer = nullptr;

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
        else{
            std::cerr << "Unable to create window" << std::endl;
        }

        renderer = SDL_CreateRenderer(window_, -1, 0);
        if (renderer){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer created" << std::endl;
        }
        else{
            std::cerr << "Unable to create renderer" << std::endl;
        }

        isRunning_ = true;
        player.addComponent<TransformComponent>(-4, -14, .125);
        player.addComponent<SpriteComponent>();
        player.getComponent<SpriteComponent>().setTexture("../assets/parachute.png");
        player.addComponent<KeyboardController>();
        player.addComponent<ColliderComponent>();

        wall.addComponent<TransformComponent>(300, 200, 200, 200, 1.0f);
        wall.addComponent<ColliderComponent>();
        wall.addComponent<SpriteComponent>();
        wall.getComponent<SpriteComponent>().setTexture("../assets/camera.png");
        manager.refresh();
        manager.update();
    }
    else{
        isRunning_ = false;
    }
    // Debug only
    // Vector2D v1(1, 2), v2(1, 1), v3;
    // std::cout << v1 << v2 << v3 << std::endl;
    // v3 = v1 + v2;
    // std::cout << v1 << v2 << v3 << std::endl;
}

void Game::handleEvents() {
    SDL_PollEvent(&event);
    switch (event.type){
    case SDL_QUIT:
        isRunning_ = false;
        break;

    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) 
            isRunning_ = false;
        break;

    default:
        break;
    }
}

void Game::render() {
    SDL_RenderClear(renderer);
    manager.draw();
    SDL_RenderPresent(renderer);
}


bool previously_collision = false;
bool collide = false;
void Game::update() {
    manager.refresh();
    manager.update();
    collide = Collision::AABB(player.getComponent<ColliderComponent>().collider, 
    wall.getComponent<ColliderComponent>().collider);

    if (collide and !previously_collision){
        std::cout << "\033[1;31mCollision!!\033[0m\n";
        previously_collision = !previously_collision;
    }
    else if (!collide and previously_collision){
        std::cout << "\033[1;32mNo collision\033[0m\n";
        previously_collision = !previously_collision;
    }
}


void Game::clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window_);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}

