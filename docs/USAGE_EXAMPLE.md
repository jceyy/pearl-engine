# ECS Usage Example

## Quick Start

### 1. Setup in Game Class

```cpp
// Game.hpp
class Game {
private:
    SystemManager* systemManager_;
    EntityManager* entityManager_;
    
    // System references (optional, for direct access)
    std::shared_ptr<RenderSystem> renderSystem_;
    std::shared_ptr<PhysicsSystem> physicsSystem_;
};

// Game.cpp - init()
void Game::init() {
    // Create managers (order matters!)
    systemManager_ = new SystemManager(nullptr);  // Temp null
    entityManager_ = new EntityManager(systemManager_);
    
    // Register systems
    renderSystem_ = systemManager_->registerSystem<RenderSystem>(entityManager_);
    physicsSystem_ = systemManager_->registerSystem<PhysicsSystem>(entityManager_);
    
    // Create entities...
}
```

### 2. Game Loop

```cpp
void Game::update() {
    // Update all systems
    systemManager_->update();
    
    // Cleanup destroyed entities (once per frame)
    entityManager_->refresh();
}

void Game::render() {
    SDL_RenderClear(renderer);
    
    // Render all systems
    systemManager_->draw();
    
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    delete entityManager_;
    delete systemManager_;
}
```

### 3. Creating Entities

```cpp
// Create a player entity
Entity& player = entityManager_->addEntity();

// Add components (triggers automatic system registration)
player.addComponent<TransformComponent>(100.0f, 100.0f);
player.addComponent<SpriteComponent>("player_texture");
player.addComponent<PhysicsComponent>(1.0f, 0.98f);  // mass, drag

// Optional: add to gameplay group
player.addGroup(Game::groupPlayers);

// The entity is NOW automatically:
// - In RenderSystem (has Transform + Sprite)
// - In PhysicsSystem (has Transform + Physics)
```

### 4. Entity Lifecycle

```cpp
// Modify entity at runtime
if (powerUp) {
    // Adding component automatically updates system lists
    player.addComponent<InvulnerableComponent>(5.0f);  // 5 sec duration
}

// Destroy entity
enemy.destroy();

// Cleanup happens in next refresh() call
entityManager_->refresh();  // Call once per frame
```

## Example: Custom Physics System

```cpp
// PhysicsSystem.hpp
#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "Systems/Systems.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/PhysicsComponent.hpp"

class PhysicsSystem : public System {
public:
    PhysicsSystem(EntityManager* entityManager);
    ~PhysicsSystem() = default;
    
    void update() override;
    void draw() override;
    
    void setGravity(const Vector2D& gravity);
    
private:
    Vector2D gravity_;
    float deltaTime_;
};

#endif
```

```cpp
// PhysicsSystem.cpp
#include "Systems/PhysicsSystem.hpp"

PhysicsSystem::PhysicsSystem(EntityManager* entityManager) 
    : System(entityManager), 
      gravity_(0.0f, 9.8f),
      deltaTime_(0.016f) {
    
    // Define what components this system needs
    signature_.set(getComponentTypeID<TransformComponent>());
    signature_.set(getComponentTypeID<PhysicsComponent>());
}

void PhysicsSystem::update() {
    // Get entities matching this system's signature
    auto& entities = entityManager_->getEntitiesForSystem(SystemID::value<PhysicsSystem>());
    
    // Process each entity
    for (Entity* entity : entities) {
        auto& transform = entity->getComponent<TransformComponent>();
        auto& physics = entity->getComponent<PhysicsComponent>();
        
        // Apply gravity
        physics.acceleration += gravity_ * physics.mass;
        
        // Update velocity
        physics.velocity += physics.acceleration * deltaTime_;
        
        // Apply drag
        physics.velocity *= physics.drag;
        
        // Update position
        transform.position += physics.velocity * deltaTime_;
        
        // Reset acceleration
        physics.acceleration = Vector2D(0, 0);
    }
}

void PhysicsSystem::draw() {
    // Optional: debug rendering
    #ifdef DEBUG_PHYSICS
    auto& entities = entityManager_->getEntitiesForSystem(SystemID::value<PhysicsSystem>());
    for (Entity* entity : entities) {
        auto& transform = entity->getComponent<TransformComponent>();
        // Draw velocity vectors, etc.
    }
    #endif
}

void PhysicsSystem::setGravity(const Vector2D& gravity) {
    gravity_ = gravity;
}
```

## Advanced: Multi-Component Systems

```cpp
// Collision System needs Transform + Collider
class CollisionSystem : public System {
public:
    CollisionSystem(EntityManager* entityManager) : System(entityManager) {
        signature_.set(getComponentTypeID<TransformComponent>());
        signature_.set(getComponentTypeID<ColliderComponent>());
    }
    
    void update() override {
        auto& entities = entityManager_->getEntitiesForSystem(
            SystemID::value<CollisionSystem>()
        );
        
        // Check collisions between all entities
        for (size_t i = 0; i < entities.size(); ++i) {
            for (size_t j = i + 1; j < entities.size(); ++j) {
                checkCollision(entities[i], entities[j]);
            }
        }
    }
    
private:
    void checkCollision(Entity* a, Entity* b) {
        auto& colA = a->getComponent<ColliderComponent>();
        auto& colB = b->getComponent<ColliderComponent>();
        
        if (AABB(colA.bounds, colB.bounds)) {
            // Handle collision...
        }
    }
};
```

## Performance Tips

### 1. Batch Component Additions

```cpp
// ❌ BAD: Triggers signature change 3 times
Entity& entity = manager.addEntity();
entity.addComponent<TransformComponent>();  // Update systems
entity.addComponent<SpriteComponent>();     // Update systems
entity.addComponent<PhysicsComponent>();    // Update systems

// ✅ GOOD: Still triggers 3 times, but happens rarely (entity creation)
// This is already optimal - signature updates happen on component add
```

### 2. Use Entity Groups for Fast Access

```cpp
// Systems process ALL entities with matching signature
// Use groups for gameplay-specific queries

enum GameGroups {
    groupPlayers,
    groupEnemies,
    groupProjectiles
};

// In collision detection:
auto& players = entityManager_->getGroup(groupPlayers);
auto& projectiles = entityManager_->getGroup(groupProjectiles);

// Only check player-projectile collisions
for (auto* player : players) {
    for (auto* proj : projectiles) {
        if (collides(player, proj)) {
            proj->destroy();
        }
    }
}
```

### 3. Call refresh() Once Per Frame

```cpp
void Game::update() {
    handleEvents();
    
    // Systems may call entity.destroy()
    systemManager_->update();
    
    // Cleanup all destroyed entities at once
    entityManager_->refresh();  // ✅ Once per frame
}
```

### 4. System Update Order

```cpp
// Register systems in the order they should update
void Game::init() {
    // 1. Input first
    auto inputSystem = systemManager_->registerSystem<InputSystem>(entityManager_);
    
    // 2. Game logic
    auto aiSystem = systemManager_->registerSystem<AISystem>(entityManager_);
    
    // 3. Physics
    auto physicsSystem = systemManager_->registerSystem<PhysicsSystem>(entityManager_);
    
    // 4. Collision
    auto collisionSystem = systemManager_->registerSystem<CollisionSystem>(entityManager_);
    
    // 5. Animation
    auto animSystem = systemManager_->registerSystem<AnimationSystem>(entityManager_);
    
    // 6. Render last
    auto renderSystem = systemManager_->registerSystem<RenderSystem>(entityManager_);
}

// Systems update in registration order
systemManager_->update();  // Input → AI → Physics → Collision → Animation → Render
```

## Migration from Old Code

If you have old components with `update()` and `draw()` methods:

```cpp
// Old component (BAD)
class SpriteComponent : public Component {
    void update() override {
        // Animation logic...
    }
    
    void draw() override {
        // Rendering logic...
    }
};

// New component (GOOD) - Pure data
class SpriteComponent : public Component {
    std::string textureID;
    SDL_Rect srcRect;
    SDL_FRect dstRect;
    SDL_RendererFlip flip;
    
    // No logic, just data
};

// Logic moved to systems
class AnimationSystem : public System {
    void update() override {
        // Update animation frames...
    }
};

class RenderSystem : public System {
    void draw() override {
        // Render all sprites...
    }
};
```

## Debugging

```cpp
// Check which systems an entity is registered with
void debugEntity(Entity* entity) {
    ComponentSignature sig = entity->getComponentSignature();
    
    std::cout << "Entity " << entity << " signature: " << sig << std::endl;
    
    // Check each system
    for (size_t i = 0; i < SystemID::maxSystemID; ++i) {
        auto& sysEntities = entityManager_->getEntitiesForSystem(i);
        auto it = std::find(sysEntities.begin(), sysEntities.end(), entity);
        
        if (it != sysEntities.end()) {
            std::cout << "  - In system " << i << std::endl;
        }
    }
}
```
