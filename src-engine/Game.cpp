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
auto& tile0(manager.addEntity());
auto& tile1(manager.addEntity());
auto& tile2(manager.addEntity());
auto& wall(manager.addEntity());
auto& player(manager.addEntity());

enum groupLabels : std::size_t{
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders
};





SDL_Renderer* Game::renderer = nullptr;
std::vector<ColliderComponent*> Game::colliders;

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

        // Load objects
        TileMap::loadMap("assets/map16x16.map", 16, 16);

        player.addComponent<TransformComponent>(-4, -14, 2);
        player.addComponent<SpriteComponent>(true);
        // player.getComponent<SpriteComponent>().setTexture("assets/parachute.png");
        player.getComponent<SpriteComponent>().setTexture("assets/player_anims.png", 32, 32);
        player.addComponent<KeyboardController>();
        player.addComponent<ColliderComponent>("player");
        player.addGroup(groupPlayers);

        // wall.addComponent<TransformComponent>(300, 200, 200, 200, .25f);
        // wall.addComponent<ColliderComponent>("camera");
        // wall.addComponent<SpriteComponent>();
        // wall.getComponent<SpriteComponent>().setTexture("assets/camera.png");
        // wall.addGroup(groupMap);
        
        manager.refresh();
        manager.update();
    }
    else{
        isRunning_ = false;
    }
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

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

void Game::render() {
    SDL_RenderClear(renderer);

    for (auto& t : tiles){
        t->draw();
    }
    for (auto& e : enemies){
        e->draw();
    }
    for (auto& p : players){
        p->draw();
    }

    // manager.draw();
    SDL_RenderPresent(renderer);
}


void Game::update() {
    manager.refresh();
    manager.update();


    for (auto cc : colliders) {
        if (cc != &player.getComponent<ColliderComponent>())
            Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
    }
}


void Game::clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window_);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}


void Game::addTile(float x, float y, int id){
    auto &tile(manager.addEntity());
    tile.addComponent<TileComponent>(x, y, 32, 32, id);
    tile.addGroup(groupMap);
}