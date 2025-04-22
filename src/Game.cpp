#include <sstream>
#include "Game.hpp"
#include "TextureManager.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"
#include "Types.hpp"
#include "Collision.hpp"
#include "TileMap.hpp"
#include "Logging.hpp"


/** 
 * Upcoming upgrades :
 * - Decoupling of update loop from render loop
 * - Text for UI
 * - Stop giving sizes in code! Set reference resolution, and then have everything depend on that
 * - Sprite component (pseudo) texture loading from constructor
 * - Error management
 * - Custom collider per frame
 * - Create UI containers
 * - Menus as a "level" and have a scene manager to switch levels
 * - Have removeGroup also remove the entity from the grouped entities
 * - Automatically remove inactive entities from grouped entities
**/

using namespace PRL;

EntityManager manager;
SDL_Event Game::event;
TileMap* tileMap;
auto& player(manager.addEntity());
auto& label(manager.addEntity());
auto& ball1(manager.addEntity());

SDL_Renderer* Game::renderer = nullptr;
bool Game::isRunning = false;
SDL_FRect Game::camera = {0, 0, 800, 640};
AssetManager* Game::assetManager = new AssetManager(&manager);

Game::Game() {}

Game::~Game() {}

void Game::init(const std::string& title, int xpos, int ypos, int width, int height, bool fullscreen) {
    
    int windowFlags = 0;
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        PRL::Logging::log("SDL subsystems initialized", "PRL::Game::init()");
        if (fullscreen){
            windowFlags = SDL_WINDOW_FULLSCREEN;
        }
        window_ = SDL_CreateWindow(title.c_str(), xpos, ypos, width, height, windowFlags);
        if (window_){
            PRL::Logging::log("Window created", "PRL::Game::init()");
        }
        else{
            PRL::Logging::err("Unable to create window : " + std::string(SDL_GetError()), "PRL::Game::init()");
        }
        // SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
        // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        renderer = SDL_CreateRenderer(window_, -1, 0 | SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer){
            SDL_SetRenderDrawColor(renderer, 236, 236, 236, 255);
            SDL_RendererInfo info;
            SDL_GetRendererInfo(renderer, &info);
            PRL::Logging::log("Renderer created with driver : " + std::string(info.name), "PRL::Game::init()");
        }
        else{
            PRL::Logging::err("Unable to create renderer : " + std::string(SDL_GetError()), "PRL::Game::init()");
        }

        if (TTF_Init() == -1){
            PRL::Logging::err("Unable to initialize SDL_TTF : " + std::string(SDL_GetError()), "PRL::Game::init()");
        }
        isRunning = true;

        // Load objects
        assetManager->addTexture("terrain", "assets/terrain_ss.png");
        assetManager->addTexture("terrain3", "assets/terrain3.png");
        assetManager->addTexture("player", "assets/player_anims.png");
        assetManager->addTexture("projectile", "assets/proj.png");
        assetManager->addTexture("circle", "assets/circle.png");
        assetManager->addFont("baseFont", "assets/font.ttf", 16);

        // assetManager->createProjectile(Vector2D(500, 500), Vector2D(2, 0), 500, 2, "projectile");

        // tileMap = new TileMap("terrain", 2);
        // tileMap->loadMap("assets/map2.map", 10);
        tileMap = new TileMap("terrain3", 2);
        tileMap->loadMap("assets/map3.map", 10);

        player.addComponent<TransformComponent>(400, 320, 3);
        player.addComponent<SpriteComponent>("", true);
        player.getComponent<SpriteComponent>().setTexture("player", 32, 32);
        player.addComponent<KeyboardController>();
        // player.addComponent<ColliderComponent>("player");
        player.addGroup(groupPlayers);

        ball1.addComponent<TransformComponent>(600, 400, 1);
        ball1.addComponent<SpriteComponent>("test", false);
        ball1.getComponent<SpriteComponent>().setTexture("circle", 16, 16);
        ball1.addComponent<PhysicsComponent>();
        ball1.addGroup(groupPlayers);

        SDL_Color colorWhite = {255, 255, 255, 255};
        label.addComponent<UILabel>(10, 10, "Test String", "baseFont", colorWhite);
        
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
auto& projectiles(manager.getGroup(Game::groupProjectiles));


void Game::handleEvents() {
    SDL_PollEvent(&event);
    switch (event.type){
    case SDL_QUIT:
        isRunning = false;
        break;

    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_SPACE){
            assetManager->createProjectile(player.getComponent<TransformComponent>().position - Vector2D(33, 0),
                                            Vector2D(-2, 0), 200, 2, "projectile");

        }
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
    for (auto& p : projectiles){
        p->draw();
    }
    label.draw();

    SDL_RenderPresent(renderer);
}


void Game::update() {
    current_time_us_ = static_cast<Uint64>(SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency());
    // SDL_FRect playerCol = player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player.getComponent<TransformComponent>().position;
    
    std::stringstream ss;
    ss << "Player position: " << playerPos;
    label.getComponent<UILabel>().setLabelText(ss.str(), "baseFont");

    manager.refresh();
    manager.update();

    // for (auto& c : colliders) {
    //     SDL_FRect cCol = c->getComponent<ColliderComponent>().collider;
    //     if (Collision::AABB(cCol, playerCol)){
    //         player.getComponent<TransformComponent>().position = playerPos;
    //     }
    // }

    // for (auto& p : projectiles) {
    //     if (Collision::AABB(player.getComponent<ColliderComponent>().collider,
    //         p->getComponent<ColliderComponent>().collider)) {
    //             p->destroy();
    //         }
    // }
    
    camera.x = player.getComponent<TransformComponent>().position.x-400;
    camera.y = player.getComponent<TransformComponent>().position.y-320;

    if (camera.x < 0) camera.x = 0;
    else if (camera.x > camera.w) camera.x = camera.w;
    if (camera.y < 0) camera.y = 0;
    else if (camera.y > camera.h) camera.y = camera.h;
}


void Game::clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window_);
    SDL_Quit();
    PRL::Logging::log("Game cleaned", "PRL::Game::clean()");
    SDL_Delay(1000); // Temporary delay for logging visibility
}

Uint64 Game::current_time_us_ = 0;

Uint64 Game::current_time_us(){
    return current_time_us_;
}
