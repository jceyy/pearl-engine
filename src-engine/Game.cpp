#include "Game.hpp"
#include "TextureManager.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"
#include "Types.hpp"
#include "Collision.hpp"
#include "TileMap.hpp"

EntityManager manager;
SDL_Event Game::event;
TileMap* tileMap;
// auto& tile0(manager.addEntity());
// auto& tile1(manager.addEntity());
// auto& tile2(manager.addEntity());
// auto& wall(manager.addEntity());
auto& player(manager.addEntity());

// const std::string mapFile = "assets/terrain_ss.png";

// std::vector<ColliderComponent*> Game::colliders;
SDL_Renderer* Game::renderer = nullptr;
bool Game::isRunning = false;
SDL_FRect Game::camera = {0, 0, 800, 640};


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

        isRunning = true;

        // Load objects
        tileMap = new TileMap("assets/terrain_ss.png", 32, 2);
        tileMap->loadMap("assets/map.map", 25, 20);

        player.addComponent<TransformComponent>(400, 320, 3);
        player.addComponent<SpriteComponent>(true);
        // player.getComponent<SpriteComponent>().setTexture("assets/parachute.png");
        player.getComponent<SpriteComponent>().setTexture("assets/player_anims.png", 32, 32);
        player.addComponent<KeyboardController>();
        player.addComponent<ColliderComponent>("player");
        player.addGroup(groupPlayers);
        
        manager.refresh();
        manager.update();
    }
    else{
        isRunning = false;
    }
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& players(manager.getGroup(Game::groupPlayers));

void Game::handleEvents() {
    SDL_PollEvent(&event);
    switch (event.type){
    case SDL_QUIT:
        isRunning = false;
        break;

    default:
        break;
    }
}


void Game::render() {
    SDL_RenderClear(renderer);

    for (auto& t : tiles){
        t->draw();
    }
    for (auto& c : colliders){
        c->draw();
    }
    for (auto& p : players){
        p->draw();
    }

    SDL_RenderPresent(renderer);
}


void Game::update() {

    SDL_FRect playerCol = player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player.getComponent<TransformComponent>().position;
    
    manager.refresh();
    manager.update();

    for (auto& c : colliders) {
        SDL_FRect cCol = c->getComponent<ColliderComponent>().collider;
        if (Collision::AABB(cCol, playerCol)){
            player.getComponent<TransformComponent>().position = playerPos;
        }
    }
    
    camera.x = player.getComponent<TransformComponent>().position.x-400;
    camera.y = player.getComponent<TransformComponent>().position.y-320;

    if (camera.x < 0) camera.x = 0;
    else if (camera.x > camera.w) camera.x = camera.w;
    if (camera.y < 0) camera.y = 0;
    else if (camera.y > camera.h) camera.y = camera.h;


    // Vector2D playerVel = player.getComponent<TransformComponent>().velocity;
    // int playerSpeed = player.getComponent<TransformComponent>().speed;

    // for (auto t : tiles){
    //     t->getComponent<TileComponent>().dstRect_.x += -playerVel.x * playerSpeed;
    //     t->getComponent<TileComponent>().dstRect_.y += -playerVel.y * playerSpeed;
    // }
    // for (auto cc : colliders) {
    //     if (cc != &player.getComponent<ColliderComponent>())
    //         Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
    // }
}


void Game::clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window_);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}

