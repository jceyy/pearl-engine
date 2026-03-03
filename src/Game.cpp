#include <sstream>
#include "Core.hpp"
#include "Game.hpp"
#include "TextureManager.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"
#include "Types.hpp"
#include "Collision.hpp"
#include "TileMap.hpp"
#include "Logging.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/AnimationSystem.hpp"

/** 
 * Upcoming upgrades :
 * - Decoupling of update loop from render loop
 * - Text for UI
 * - Stop giving sizes in code! Set reference resolution, and then have everything depend on that
 * - Sprite component (pseudo) texture loading from constructor
 * - Error management
 * - Custom collider per frame
 * - Create UI containers
 * - Menus as a "level" and have a scene entityManager to switch levels
 * - Have removeGroup also remove the entity from the grouped entities
 * - Automatically remove inactive entities from grouped entities
**/

using namespace std;
namespace PRL {

SDL_Event Game::event;
EntityManager& entityManager = Core::getEntityManager();
SystemManager& systemManager = Core::getSystemManager();
AssetManager& assetManager  = Core::getAssetManager();

Entity* player = nullptr;
Entity* tileMap = nullptr;

Entity* label = nullptr;
Entity* ball1 = nullptr;

bool Game::isRunning = false;
SDL_FRect Game::camera = {0, 0, 800, 640};

size_t Game::instanceCount_ = 0;


// Methods
Game::Game() {
    instanceCount_++;
}

Game::~Game() {
    clean();
    instanceCount_--;
}

void Game::init() {
    isRunning = true;
    SDL_SetRenderDrawColor(Core::renderer, 236, 236, 236, 255);

    // Register systems
    systemManager.registerSystem<AnimationSystem>();
    systemManager.registerSystem<RenderSystem>();

    // Load objects
    // assetManager->addTexture("terrain", "assets/terrain_ss.png");
    // assetManager->addTexture("terrain3", "assets/terrain3.png");
    // assetManager->addTexture("player", "assets/player_anims.png");
    assetManager.addAsset("player", "assets/player_anims.dat");

    // assetManager->addTexture("projectile", "assets/proj.png");
    // assetManager->addTexture("circle", "assets/circle.png");
    assetManager.addFont("baseFont", "assets/font.ttf", 16);

    // assetManager->createProjectile(Vector2D(500, 500), Vector2D(2, 0), 500, 2, "projectile");

    // Create entities
    player = &entityManager.addEntity();
    tileMap = &entityManager.addEntity();

    label = &entityManager.addEntity();
    ball1 = &entityManager.addEntity();

    // tileMap = new TileMap("terrain", 2);
    // tileMap->loadMap("assets/map2.map", 10);

    // tileMap = new TileMap("terrain3", 2);
    // tileMap->loadMap("assets/map3.map", 10);

    player->addComponent<TransformComponent>(250, 300, 4.0, 4.0, -45);
    player->addComponent<SpriteComponent>("player");
    player->addComponent<AnimationComponent>("player.idle");
    // player.addComponent<KeyboardController>();
    // player.addComponent<ColliderComponent>("player");
    player->addGroup(groupPlayers);

    tileMap->addComponent<TileMapComponent>();
    tileMap->getComponent<TileMapComponent>().map.loadMap("assets/testmap.map");

    // ball1.addComponent<TransformComponent>(600, 400, 1);
    // ball1.addComponent<SpriteComponent>("test");
    // ball1.getComponent<SpriteComponent>().setTexture("circle", 16, 16);
    // ball1.addComponent<PhysicsComponent>();
    // ball1.addGroup(groupPlayers);

    // SDL_Color colorWhite = {255, 255, 255, 255};
    // label.addComponent<UILabel>(10, 10, "Test String", "baseFont", colorWhite);

    entityManager.refresh();
    entityManager.update();
    PRL::Logging::log("Game initialized", "PRL::Game::init");
}

// auto& tiles(entityManager.getGroup(Game::groupMap));
// auto& colliders(entityManager.getGroup(Game::groupColliders));
// auto& players(entityManager.getGroup(Game::groupPlayers));
// auto& projectiles(entityManager.getGroup(Game::groupProjectiles));


void Game::handleEvents() {
    while(SDL_PollEvent(&event)) {
        switch (event.type){
        case SDL_QUIT:
            isRunning = false;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_SPACE){
                // assetManager->createProjectile(player.getComponent<TransformComponent>().position - Vector2D(33, 0),
                //                                 Vector2D(-2, 0), 200, 2, "projectile");
            }
            else if (event.key.keysym.sym == SDLK_ESCAPE){
                isRunning = false;
            }
            break;

        default:
            break;
        }
    }

    const Uint8* keystates = SDL_GetKeyboardState(nullptr);
    int moveSpeed = 5;
    const float deg2rad = M_PI / 180.0f;

    auto walkAnim = assetManager.getAnimationHandle("player.walk");
    auto idleAnim = assetManager.getAnimationHandle("player.idle");

    auto& playerTransform = player->getComponent<TransformComponent>();
    
    Vec2D<float> unitDirection(0, 0);
    
    if (keystates[SDL_SCANCODE_LSHIFT]) {
        moveSpeed *= 2;
    }
    if (keystates[SDL_SCANCODE_UP] || keystates[SDL_SCANCODE_W]){
        unitDirection.x+=sin(playerTransform.rotation * deg2rad);
        unitDirection.y-=cos(playerTransform.rotation * deg2rad);
    }
    if (keystates[SDL_SCANCODE_DOWN] || keystates[SDL_SCANCODE_S]){
        unitDirection.x-=sin(playerTransform.rotation * deg2rad);
        unitDirection.y+=cos(playerTransform.rotation * deg2rad);
    }
    if (keystates[SDL_SCANCODE_LEFT] || keystates[SDL_SCANCODE_A]){
        player->getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_HORIZONTAL;
        unitDirection.x-=cos(playerTransform.rotation * deg2rad);
        unitDirection.y-=sin(playerTransform.rotation * deg2rad);
    }
    if (keystates[SDL_SCANCODE_RIGHT] || keystates[SDL_SCANCODE_D]){
        player->getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_NONE;
        unitDirection.x+=cos(playerTransform.rotation * deg2rad);
        unitDirection.y+=sin(playerTransform.rotation * deg2rad);
    }
    if (keystates[SDL_SCANCODE_Q]){
        playerTransform.rotation -= 45.0/4;
    }
    if (keystates[SDL_SCANCODE_E]){
        playerTransform.rotation += 45.0/4;
    }
    if (keystates[SDL_SCANCODE_R]){
        playerTransform.rotation = 0;
        int textureWidth = assetManager.getTextureAsset(player->getComponent<SpriteComponent>().textureHandle)->regions[0].w;
        int textureHeight = assetManager.getTextureAsset(player->getComponent<SpriteComponent>().textureHandle)->regions[0].h;
        textureWidth *= playerTransform.scale.x;
        textureHeight *= playerTransform.scale.y;
        playerTransform.position.x = Core::screenSize.x / 2 - textureWidth / 2;
        playerTransform.position.y = Core::screenSize.y / 2 - textureHeight / 2;
    }


    if (!keystates[SDL_SCANCODE_RIGHT] && !keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_UP] && !keystates[SDL_SCANCODE_DOWN] &&
        !keystates[SDL_SCANCODE_S] && !keystates[SDL_SCANCODE_W] && !keystates[SDL_SCANCODE_A] && !keystates[SDL_SCANCODE_D]){
        if (player->getComponent<AnimationComponent>().animHandle != idleAnim){
            player->getComponent<AnimationComponent>().setHandle(idleAnim);
        }
    }
    else {
        if (player->getComponent<AnimationComponent>().animHandle != walkAnim){
            player->getComponent<AnimationComponent>().setHandle(walkAnim);
        }
    }

    // Apply movement
    unitDirection.normalize();
    playerTransform.position += unitDirection * moveSpeed;

}


void Game::render() {
    SDL_RenderClear(PRL::Core::renderer);
    systemManager.draw();
    SDL_RenderPresent(PRL::Core::renderer);
}


void Game::update() {
    PRL::Core::updateCurrentTime();
    // SDL_FRect playerCol = player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player->getComponent<TransformComponent>().position;
    
    std::stringstream ss;
    ss << "Player position: " << playerPos;
    if (label->hasComponent<UILabel>())
        label->getComponent<UILabel>().setLabelText(ss.str(), "baseFont");

    auto& entityManager = PRL::Core::getEntityManager();
    entityManager.refresh();
    entityManager.update();

    // for (auto& c : colliders) {
    //     SDL_FRect cCol = c->getComponent<ColliderComponent>().collider;
    //     if (Collision::AABB(cCol, playerCol)){
    //         player->getComponent<TransformComponent>().position = playerPos;
    //     }
    // }

    // for (auto& p : projectiles) {
    //     if (Collision::AABB(player->getComponent<ColliderComponent>().collider,
    //         p->getComponent<ColliderComponent>().collider)) {
    //             p->destroy();
    //         }
    // }
    
    camera.x = player->getComponent<TransformComponent>().position.x - 400;
    camera.y = player->getComponent<TransformComponent>().position.y - 320;

    if (camera.x < 0) camera.x = 0;
    else if (camera.x > camera.w) camera.x = camera.w;
    if (camera.y < 0) camera.y = 0;
    else if (camera.y > camera.h) camera.y = camera.h;

    PRL::Core::getSystemManager().update();
}


void Game::clean() {
    PRL::Logging::log("Game cleaned", "PRL::Game::clean()");
}

} // namespace PRL