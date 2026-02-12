# ECS Architecture - Performance Optimized

## Overview

This ECS implementation uses an **EntityManager-centric** approach for optimal performance. Entity lists per system are maintained centrally and updated incrementally when entity signatures change.

## Core Design Principles

### 1. **EntityManager Owns Entity-to-System Mappings**

```cpp
class EntityManager {
    std::array<std::vector<Entity*>, SystemID::maxSystemID> entitiesPerSystem_;
};
```

- **Single source of truth** for which entities belong to which systems
- **Better cache locality** - all entity lists stored contiguously
- **No duplicate storage** - systems don't store their own entity lists

### 2. **Incremental Updates (Zero Per-Frame Overhead)**

When an entity's component signature changes (add/remove component):

```cpp
void EntityManager::onEntitySignatureChanged(Entity* entity, ComponentSignature signature) {
    // For each registered system:
    //   - Check if entity matches system's signature
    //   - Add to list if matched and not present
    //   - Remove from list if no longer matches
}
```

✅ **Performance benefit**: No iteration over all entities every frame  
✅ **Update happens only when**: Components are added/removed (rare)

### 3. **Systems Process, Don't Store**

```cpp
class System {
protected:
    ComponentSignature signature_;        // What entities I care about
    EntityManager* entityManager_;     // Where to get entities from
};

class RenderSystem : public System {
    void draw() override {
        // Get entities matching this system's signature
        auto& entities = entityManager_->getEntitiesForSystem(SystemID::value<RenderSystem>());
        
        for (Entity* entity : entities) {
            // Process entity...
        }
    }
};
```

## How the System Works

### Entity Lifecycle

1. **Entity Created**
   ```cpp
   Entity& entity = entityManager.addEntity();
   // Entity has no components, not in any system lists
   ```

2. **Component Added**
   ```cpp
   entity.addComponent<TransformComponent>(x, y);
   entity.addComponent<SpriteComponent>();
   // Triggers: Entity::notifySignatureChange_()
   //   -> EntityManager::onEntitySignatureChanged()
   //   -> Check all systems, add to matching ones
   ```

3. **System Updates**
   ```cpp
   systemManager.update();  // Calls each system's update()
   // Each system retrieves its entities from EntityManager
   ```

4. **Entity Destroyed**
   ```cpp
   entity.destroy();
   entityManager.refresh();  // Removes from all lists
   ```

### Signature Matching Logic

```cpp
// Entity signature: has {Transform, Sprite}
ComponentSignature entitySig = 0b00000011;

// System signature: requires {Transform}
ComponentSignature systemSig = 0b00000001;

// Check if entity matches system
bool matches = (entitySig & systemSig) == systemSig;  // true!
```

## Performance Characteristics

### Time Complexity

| Operation | Complexity | Notes |
|-----------|------------|-------|
| Add component | O(S) | S = number of systems (~constant, max 32) |
| Remove component | O(S) | |
| System update | O(E) | E = entities matching that system |
| Entity refresh | O(G + S + 1) | G = groups, S = systems |

### Best Practices

1. **Batch component additions** before notifying signature changes
2. **Use entity groups** for gameplay categories (players, enemies)
3. **Call refresh() once per frame**, not after every destroy
4. **Keep system count reasonable** (< 32, practically < 10)

## Creating a New System

```cpp
// 1. Define system signature in constructor
class PhysicsSystem : public System {
public:
    PhysicsSystem(EntityManager* entityManager) : System(entityManager) {
        signature_.set(getComponentTypeID<TransformComponent>());
        signature_.set(getComponentTypeID<PhysicsComponent>());
    }
    
    void update() override {
        // 2. Get entities from EntityManager
        auto& entities = entityManager_->getEntitiesForSystem(
            SystemID::value<PhysicsSystem>()
        );
        
        // 3. Process entities with guaranteed components
        for (Entity* entity : entities) {
            auto& transform = entity->getComponent<TransformComponent>();
            auto& physics = entity->getComponent<PhysicsComponent>();
            
            // Apply physics...
            transform.position += physics.velocity * deltaTime;
        }
    }
    
    void draw() override {}  // No rendering
};
```

## Register System in Game

```cpp
// Game initialization
SystemManager systemManager(&entityManager);
EntityManager entityManager(&systemManager);

auto renderSystem = systemManager.registerSystem<RenderSystem>(&entityManager);
auto physicsSystem = systemManager.registerSystem<PhysicsSystem>(&entityManager);

// Game loop
while (running) {
    systemManager.update();  // Updates all systems
    systemManager.draw();    // Renders all systems
    entityManager.refresh(); // Cleanup dead entities
}
```

## Comparison: Alternative Approaches

### ❌ System Stores Entities (Old Approach)
```cpp
class System {
    std::vector<Entity*> entities;  // Duplicate storage per system
};
```
**Problems**:
- Memory overhead (N systems × M entities)
- Must update every system when entity changes
- Systems coupled to entity storage

### ❌ Query Every Frame
```cpp
void System::update() {
    for (Entity* e : allEntities) {
        if (matches(e)) { /* process */ }
    }
}
```
**Problems**:
- O(E × S) per frame (E = all entities, S = systems)
- No caching of matching entities
- Poor cache locality

### ✅ EntityManager-Centric (Current)
- Update only on signature changes (rare)
- Central entity storage with indexing
- Systems remain stateless and simple

## Memory Layout

```
EntityManager
├── entities_ [unique_ptr<Entity>...]          // Owned entities
├── groupedEntities_[32][Entity*...]           // Grouped by gameplay
└── entitiesPerSystem_[32][Entity*...]         // Indexed by SystemID
                                                   ↑
SystemManager                                      |
├── systems_[32][shared_ptr<System>]               |
└── signatures_[32][ComponentSignature]               |
                                                   |
System (e.g. RenderSystem)                         |
├── signature_ = 0b00000100                        |
└── entityManager_ ────────────────────────────────┘
     Queries: getEntitiesForSystem(SystemID)
```

## When to Use This Pattern

✅ **Good for:**
- Real-time games where per-frame performance matters
- Engines with many entities (100s-1000s)
- Stable component signatures (don't add/remove components often)

⚠️ **Consider alternatives if:**
- Entities frequently change components every frame
- Very few entities (< 50) - simpler approaches suffice
- Systems need complex queries beyond signature matching

## Future Optimizations

1. **Component pools**: Store components contiguously, not in entities
2. **Archetypes**: Group entities with identical signatures
3. **Multi-threading**: Parallel system updates with dependency graph
4. **Spatial partitioning**: For physics/collision systems
