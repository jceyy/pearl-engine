#ifndef PRL_COLLISION_SYSTEM_HPP_INCLUDED
#define PRL_COLLISION_SYSTEM_HPP_INCLUDED

#include <vector>
#include <utility>
#include "Systems.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/VelocityComponent.hpp"
#include "ECS/RigidBodyComponent.hpp"
#include "TileMap.hpp"

// TODOs (left in comments in the code):
// Swept / Speculative / SweptTOI algorithm paths
// Circle and Capsule narrow-phase (currently uses bounding AABB)
// Tile CollisionType metadata for slopes and one-way platforms
// Restitution from PhysicsComponent instead of hardcoded 0


namespace PRL {

// Written into contacts_ each frame. entityB == nullptr means the contact was with a tile.
struct ContactEvent {
    Entity*  entityA     = nullptr;
    Entity*  entityB     = nullptr;   // nullptr = tilemap contact
    Vector2D normal;                  // unit vector: direction entityA should move to separate
    float    penetration = 0.f;
};

class CollisionSystem : public System {
public:
    explicit CollisionSystem(ComponentSignature signature =
        ComponentSignature::create<TransformComponent, RigidBodyComponent>());
    ~CollisionSystem();

    // Register a TileMap as a static collision world.
    // solidLayer: index of the tile layer whose non-empty tiles are treated as solid.
    void addTileMap(TileMap* map, LayerID solidLayer = 0);
    void removeTileMap(TileMap* map);

    // Per-frame contact list; cleared at the start of each update().
    const std::vector<ContactEvent>& contacts() const noexcept { return contacts_; }

    // Registered static collision worlds (read by DebugDrawSystem).
    const std::vector<std::pair<TileMap*, LayerID>>& tileMaps() const noexcept { return tileMaps_; }

    void update() override;
    void draw() override {}  // debug drawing lives in DebugDrawSystem (must draw last)

    static inline size_t getInstanceCount() noexcept { return instanceCount_; }

private:
    std::vector<std::pair<TileMap*, LayerID>> tileMaps_;
    std::vector<ContactEvent>                 contacts_;

    // Returns the world-space AABB for a body (local AABB offset by entity position).
    static Collision::AABB worldAABB(const RigidBodyComponent& body,
                                     const TransformComponent& tf);

    // AABB overlap test. On contact, fills normal (direction A separates from B)
    // and penetration depth. Returns false if no overlap.
    static bool testAABBvsAABB(const Collision::AABB& a, const Collision::AABB& b,
                               Vector2D& normal, float& penetration);

    // Translates a transform by normal * penetration * fraction to resolve overlap.
    static void resolvePosition(TransformComponent& tf, const Vector2D& normal,
                                float penetration, float fraction = 1.f);

    // Reflects the velocity component along the collision normal.
    // restitution: 0 = inelastic (stop), 1 = fully elastic (bounce).
    static void resolveVelocity(VelocityComponent& vel, const Vector2D& normal,
                                float restitution);

    // Resolves one entity against all solid tiles in a registered tilemap.
    void resolveVsTileMap(Entity* entity, RigidBodyComponent& body,
                          TransformComponent& tf, TileMap& map, LayerID solidLayer);

    // Resolves a pair of entities against each other.
    void resolveVsEntity(Entity* entityA, RigidBodyComponent& bodyA, TransformComponent& tfA,
                         Entity* entityB, RigidBodyComponent& bodyB, TransformComponent& tfB);

    static size_t instanceCount_;
};

} // namespace PRL

#endif // PRL_COLLISION_SYSTEM_HPP_INCLUDED
