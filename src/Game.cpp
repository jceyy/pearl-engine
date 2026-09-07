#include <sstream>
#include <cmath>
#include "Core.hpp"
#include "Game.hpp"
#include "TextureManager.hpp"
#include "ECS/ECS.hpp"
#include "ECS/Components.hpp"
#include "Types.hpp"
#include "Collision.hpp"
#include "TileMap.hpp"
#include "Logging.hpp"
#include "Camera.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/AnimationSystem.hpp"
#include "Systems/TileMapSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/MovementSystem.hpp"

/** 
 * Upcoming upgrades :
 * - PlayerControllerComponent/System: coyote time, jump buffering, variable jump height
 *   (the jump logic below is a temporary placeholder living in Game)
 * - InputSystem with action mapping and pressed/held/released edge detection
 * - Tile collision metadata (one-way platforms, slopes, hazards)
 * - Text for UI
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

CollisionSystem* collisionSystem = nullptr;

// --- Platformer tuning (world units; 1 unit = 1 tile at reference ppu) ---
static const Vector2D playerSpawn(2.f, 5.f);
static const float runSpeed  = 7.f;   // units/s
static const float runBoost  = 1.6f;  // LSHIFT multiplier
static const float jumpSpeed = 15.f;  // initial upward velocity, units/s

// Placeholder input state until an InputSystem exists.
static bool jumpRequested = false;
static bool playerGrounded = false;

bool Game::isRunning = false;

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

    // Register systems.
    // Update order == registration order: movement integrates velocities, then
    // collisions resolve the resulting penetrations, then animations advance.
    // Draw order also follows registration: tilemap first, sprites on top.
    systemManager.registerSystem<MovementSystem>();
    collisionSystem = &systemManager.registerSystem<CollisionSystem>();
    systemManager.registerSystem<AnimationSystem>();
    systemManager.registerSystem<TileMapSystem>();
    systemManager.registerSystem<RenderSystem>();

    // Load assets
    assetManager.addAsset("player", "assets/player_anims.dat");
    assetManager.addAsset("tileset", "assets/terrain_asset.dat");
    assetManager.addFont("baseFont", "assets/font.ttf", 16);

    // Create entities
    player = &entityManager.addEntity();
    tileMap = &entityManager.addEntity();
    label = &entityManager.addEntity();

    // --- Player: transform + sprite + velocity + dynamic rigid body ---
    player->addComponent<TransformComponent>(playerSpawn.x, playerSpawn.y);
    player->addComponent<SpriteComponent>("player", "walk4");
    player->addComponent<AnimationComponent>("player.idle");
    player->addComponent<VelocityComponent>(0.f, 0.f);

    RigidBodyComponent& body = player->addComponent<RigidBodyComponent>(Collision::BodyType::Dynamic);
    // Collider = the sprite's world-space footprint (region pixels / texture ppu * scale).
    {
        float wWorld = 1.f, hWorld = 1.f;
        const SpriteComponent& sprite = player->getComponent<SpriteComponent>();
        const TransformComponent& tf = player->getComponent<TransformComponent>();
        const TextureAsset* tex = assetManager.getTextureAsset(sprite.textureHandle);
        if (tex && !tex->regions.empty() && tex->pixels_per_unit > 0) {
            wWorld = tex->regions[0].w * std::abs(tf.scale.x) / static_cast<float>(tex->pixels_per_unit);
            hWorld = tex->regions[0].h * std::abs(tf.scale.y) / static_cast<float>(tex->pixels_per_unit);
        }
        body.shape = Collision::Shape(Collision::AABB(0.f, 0.f, wWorld, hWorld));
    }
    body.responseFlags = Collision::CollisionResponseFlags::Resolve
                       | Collision::CollisionResponseFlags::Impulse;
    player->addGroup(groupPlayers);

    // --- Tilemap: rendered by TileMapSystem, solid for the CollisionSystem ---
    tileMap->addComponent<TileMapComponent>();
    TileMap& map = tileMap->getComponent<TileMapComponent>().map;
    map.loadMap("assets/testmap.map");
    collisionSystem->addTileMap(&map, 0); // layer 0 treated as solid for now

    // --- Camera: follow the player, stay inside the map ---
    {
        Camera& camera = Core::getCamera();
        const int ppu = Core::settings.render.pixelsPerUnit;
        const float mapW = map.mapSize().x * map.tileSize().x / static_cast<float>(ppu);
        const float mapH = map.mapSize().y * map.tileSize().y / static_cast<float>(ppu);
        camera.setBounds(Collision::AABB(0.f, 0.f, mapW, mapH));
        camera.setDeadZone(1.5f, 1.f);
        camera.setSmoothing(8.f);
        camera.setTarget(playerSpawn);
        camera.snapToTarget();
    }

    // SDL_Color colorWhite = {255, 255, 255, 255};
    // label.addComponent<UILabel>(10, 10, "Test String", "baseFont", colorWhite);

    entityManager.refresh();
    entityManager.update();
    Logging::log("Game initialized", "Game::init()");
}


void Game::handleEvents() {
    while(SDL_PollEvent(&event)) {
        switch (event.type){
        case SDL_QUIT:
            isRunning = false;
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_SPACE && event.key.repeat == 0){
                jumpRequested = true; // consumed in update() if grounded
            }
            else if (event.key.keysym.sym == SDLK_ESCAPE){
                isRunning = false;
            }
            else if (event.key.keysym.sym == SDLK_r){
                // Reset the player to spawn
                player->getComponent<TransformComponent>().setPos(playerSpawn.x, playerSpawn.y);
                player->getComponent<VelocityComponent>().setVel(0.f, 0.f);
                Core::getCamera().snapToTarget();
            }
            else if (event.key.keysym.sym == SDLK_c && event.key.repeat == 0){
                // Toggle hitbox debug draw for all collision bodies.
            }
            break;

        default:
            break;
        }
    }

    // Horizontal control (velocity-based; MovementSystem integrates it).
    const Uint8* keystates = SDL_GetKeyboardState(nullptr);
    float vx = 0.f;
    if (keystates[SDL_SCANCODE_LEFT]  || keystates[SDL_SCANCODE_A]) vx -= runSpeed;
    if (keystates[SDL_SCANCODE_RIGHT] || keystates[SDL_SCANCODE_D]) vx += runSpeed;
    if (keystates[SDL_SCANCODE_LSHIFT]) vx *= runBoost;

    VelocityComponent& vel = player->getComponent<VelocityComponent>();
    vel.linearVel.x = vx;

    // Facing + animation state from horizontal velocity.
    SpriteComponent& sprite = player->getComponent<SpriteComponent>();
    if (vx < 0.f) sprite.spriteFlip = SDL_FLIP_HORIZONTAL;
    else if (vx > 0.f) sprite.spriteFlip = SDL_FLIP_NONE;

    if (player->hasComponent<AnimationComponent>()) {
        auto walkAnim = assetManager.getAnimationHandle("player.walk");
        auto idleAnim = assetManager.getAnimationHandle("player.idle");
        AnimationComponent& anim = player->getComponent<AnimationComponent>();
        auto wanted = (vx != 0.f) ? walkAnim : idleAnim;
        if (anim.animHandle != wanted) anim.setHandle(wanted);
    }
}


void Game::render() {
    SDL_RenderClear(PRL::Core::renderer);
    systemManager.draw();
    SDL_RenderPresent(PRL::Core::renderer);
}


//! Runs once per fixed timestep (see apps/main.cpp); Core::getFixedDeltaTime() is the dt.
void Game::update() {
    const double dt = Core::getFixedDeltaTime();

    // Grounded = last fixed step produced a contact pushing the player up.
    // (normal is the direction the entity moves to separate; floor => (0,-1))
    playerGrounded = false;
    for (const ContactEvent& contact : collisionSystem->contacts()) {
        if (contact.entityA == player && contact.normal.y < -0.5f) {
            playerGrounded = true;
            break;
        }
    }

    // Placeholder jump — replaced by a proper PlayerController (coyote time,
    // jump buffering, variable height) in a later step.
    if (jumpRequested) {
        if (playerGrounded) {
            player->getComponent<VelocityComponent>().linearVel.y = -jumpSpeed;
        }
        jumpRequested = false;
    }

    if (label->hasComponent<UILabel>()) {
        std::stringstream ss;
        ss << "Player position: " << player->getComponent<TransformComponent>().position
           << (playerGrounded ? " [grounded]" : " [airborne]");
        label->getComponent<UILabel>().setLabelText(ss.str(), "baseFont");
    }

    entityManager.refresh();
    entityManager.update();

    // Movement -> Collision -> Animation (registration order).
    systemManager.update();

    // Camera follows the player's collider center.
    {
        Camera& camera = Core::getCamera();
        const TransformComponent& tf = player->getComponent<TransformComponent>();
        const Collision::AABB& box = player->getComponent<RigidBodyComponent>().shape.boundingAABB();
        camera.setTarget(Vector2D(tf.position.x + box.x + box.w * 0.5f,
                                  tf.position.y + box.y + box.h * 0.5f));
        camera.update(dt);
    }
}


void Game::clean() {
    Logging::log("Game cleaned", "Game::clean()");
}

} // namespace PRL
