#include "Systems/CollisionSystem.hpp"
#include "Core.hpp"
#include <algorithm>
#include <cmath>

namespace PRL {

size_t CollisionSystem::instanceCount_ = 0;

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

CollisionSystem::CollisionSystem(ComponentSignature signature) : System(signature) {
    systemName_ = "CollisionSystem";
    instanceCount_++;
}

CollisionSystem::~CollisionSystem() {
    instanceCount_--;
}

// ---------------------------------------------------------------------------
// TileMap registration
// ---------------------------------------------------------------------------

void CollisionSystem::addTileMap(TileMap* map, LayerID solidLayer) {
    for (size_t i = 0; i < tileMaps_.size(); ++i)
        if (tileMaps_[i].first == map) return;
    tileMaps_.push_back(std::make_pair(map, solidLayer));
}

void CollisionSystem::removeTileMap(TileMap* map) {
    tileMaps_.erase(
        std::remove_if(tileMaps_.begin(), tileMaps_.end(),
            [map](const std::pair<TileMap*, LayerID>& p) { return p.first == map; }),
        tileMaps_.end());
}

// ---------------------------------------------------------------------------
// Geometry helpers
// ---------------------------------------------------------------------------

Collision::AABB CollisionSystem::worldAABB(const RigidBodyComponent& body,
                                            const TransformComponent& tf) {
    const Collision::AABB& local = body.shape.boundingAABB();
    return { local.x + tf.position.x,
             local.y + tf.position.y,
             local.w,
             local.h };
}

bool CollisionSystem::testAABBvsAABB(const Collision::AABB& a, const Collision::AABB& b,
                                      Vector2D& normal, float& penetration) {
    float acx = a.x + a.w * 0.5f;
    float acy = a.y + a.h * 0.5f;
    float bcx = b.x + b.w * 0.5f;
    float bcy = b.y + b.h * 0.5f;

    float dx = acx - bcx;
    float dy = acy - bcy;

    float overlapX = (a.w + b.w) * 0.5f - std::abs(dx);
    float overlapY = (a.h + b.h) * 0.5f - std::abs(dy);

    if (overlapX <= 0.f || overlapY <= 0.f) return false;

    // Resolve along the axis of minimum penetration.
    if (overlapX < overlapY) {
        penetration = overlapX;
        normal = { dx >= 0.f ? 1.f : -1.f, 0.f };
    } else {
        penetration = overlapY;
        normal = { 0.f, dy >= 0.f ? 1.f : -1.f };
    }
    return true;
}

void CollisionSystem::resolvePosition(TransformComponent& tf, const Vector2D& normal,
                                       float penetration, float fraction) {
    tf.position.x += normal.x * penetration * fraction;
    tf.position.y += normal.y * penetration * fraction;
}

void CollisionSystem::resolveVelocity(VelocityComponent& vel, const Vector2D& normal,
                                       float restitution) {
    float vn = vel.linearVel.x * normal.x + vel.linearVel.y * normal.y;
    if (vn >= 0.f) return;  // already separating
    float impulse = -(1.f + restitution) * vn;
    vel.linearVel.x += impulse * normal.x;
    vel.linearVel.y += impulse * normal.y;
}

// ---------------------------------------------------------------------------
// Entity vs TileMap
// ---------------------------------------------------------------------------

void CollisionSystem::resolveVsTileMap(Entity* entity, RigidBodyComponent& body,
                                        TransformComponent& tf, TileMap& map,
                                        LayerID solidLayer) {
    const int ppu = Core::settings.render.pixelsPerUnit;
    const Vec2D<int> tileSize = map.tileSize();
    const Vec2D<int> mapSize  = map.mapSize();

    // Tile dimensions in world units (TransformComponent uses world units, tileSize is pixels).
    const float tsX = tileSize.x / static_cast<float>(ppu);
    const float tsY = tileSize.y / static_cast<float>(ppu);

    Collision::AABB aabb = worldAABB(body, tf);

    // Global tile range that could overlap the body AABB.
    int tileX0 = std::max(0,             static_cast<int>(std::floor(aabb.x           / tsX)));
    int tileY0 = std::max(0,             static_cast<int>(std::floor(aabb.y           / tsY)));
    int tileX1 = std::min(mapSize.x - 1, static_cast<int>(std::floor((aabb.x + aabb.w) / tsX)));
    int tileY1 = std::min(mapSize.y - 1, static_cast<int>(std::floor((aabb.y + aabb.h) / tsY)));

    for (int ty = tileY0; ty <= tileY1; ++ty) {
        for (int tx = tileX0; tx <= tileX1; ++tx) {
            // Convert global tile coord → chunk + local.
            // Access via getChunk() + local index (same pattern as TileMapSystem::draw).
            int chunkX = tx >> TileMap::CHUNK_BIT_SHIFT;
            int chunkY = ty >> TileMap::CHUNK_BIT_SHIFT;
            int localX = tx & TileMap::CHUNK_TILE_MASK;
            int localY = ty & TileMap::CHUNK_TILE_MASK;

            const TileChunk& chunk = map.getChunk(chunkX, chunkY);
            if (solidLayer >= static_cast<LayerID>(chunk.tiles.size())) continue;

            const Tile& tile = chunk.tiles[solidLayer]
                                          [localY * TileMap::CHUNK_TILE_SIZE + localX];
            if (tile.ID == Tile::emptyTileID) continue;

            // TODO: once tile CollisionType metadata exists, handle slopes and platforms here.

            Collision::AABB tileAABB { tx * tsX, ty * tsY, tsX, tsY };

            // Re-fetch body AABB: position may have shifted by earlier tile resolutions.
            aabb = worldAABB(body, tf);

            Vector2D normal;
            float    penetration;
            if (!testAABBvsAABB(aabb, tileAABB, normal, penetration)) continue;

            if (Collision::hasFlag(body.responseFlags, Collision::CollisionResponseFlags::Resolve)) {
                resolvePosition(tf, normal, penetration);
                aabb = worldAABB(body, tf);
            }

            if (Collision::hasFlag(body.responseFlags, Collision::CollisionResponseFlags::Impulse)) {
                if (entity->hasComponent<VelocityComponent>()) {
                    // restitution = 0 (inelastic) until PhysicsComponent integration is added.
                    resolveVelocity(entity->getComponent<VelocityComponent>(), normal, 0.f);
                }
            }

            contacts_.push_back({ entity, nullptr, normal, penetration });
        }
    }
}

// ---------------------------------------------------------------------------
// Entity vs Entity
// ---------------------------------------------------------------------------

void CollisionSystem::resolveVsEntity(Entity* entityA, RigidBodyComponent& bodyA,
                                       TransformComponent& tfA,
                                       Entity* entityB, RigidBodyComponent& bodyB,
                                       TransformComponent& tfB) {
    // Collision happens if at least one body's mask includes the other's layer.
    if (!(bodyA.mask & bodyB.layer) && !(bodyB.mask & bodyA.layer)) return;

    const Collision::AABB aabbA = worldAABB(bodyA, tfA);
    const Collision::AABB aabbB = worldAABB(bodyB, tfB);

    Vector2D normal;
    float    penetration;
    if (!testAABBvsAABB(aabbA, aabbB, normal, penetration)) return;

    Vector2D normalB = { -normal.x, -normal.y };

    bool dynA = (bodyA.bodyType == Collision::BodyType::Dynamic);
    bool dynB = (bodyB.bodyType == Collision::BodyType::Dynamic);

    bool resolveA = dynA && Collision::hasFlag(bodyA.responseFlags,
                                               Collision::CollisionResponseFlags::Resolve);
    bool resolveB = dynB && Collision::hasFlag(bodyB.responseFlags,
                                               Collision::CollisionResponseFlags::Resolve);

    if (resolveA && resolveB) {
        resolvePosition(tfA, normal,  penetration, 0.5f);
        resolvePosition(tfB, normalB, penetration, 0.5f);
    } else if (resolveA) {
        resolvePosition(tfA, normal, penetration, 1.f);
    } else if (resolveB) {
        resolvePosition(tfB, normalB, penetration, 1.f);
    }

    if (dynA && Collision::hasFlag(bodyA.responseFlags, Collision::CollisionResponseFlags::Impulse)
             && entityA->hasComponent<VelocityComponent>()) {
        resolveVelocity(entityA->getComponent<VelocityComponent>(), normal, 0.f);
    }
    if (dynB && Collision::hasFlag(bodyB.responseFlags, Collision::CollisionResponseFlags::Impulse)
             && entityB->hasComponent<VelocityComponent>()) {
        resolveVelocity(entityB->getComponent<VelocityComponent>(), normalB, 0.f);
    }

    contacts_.push_back({ entityA, entityB, normal, penetration });
}

// ---------------------------------------------------------------------------
// Main update
// ---------------------------------------------------------------------------

void CollisionSystem::update() {
    contacts_.clear();

    const size_t systemID = SystemID::getSystemTypeID<CollisionSystem>();
    auto& entities = entityManager_->getEntitiesForSystem(systemID);

    // --- Entity vs TileMap ---
    for (size_t i = 0; i < entities.size(); ++i) {
        Entity* entity = entities[i];
        RigidBodyComponent& body = entity->getComponent<RigidBodyComponent>();
        if (body.bodyType == Collision::BodyType::Static) continue;

        TransformComponent& tf = entity->getComponent<TransformComponent>();
        for (size_t t = 0; t < tileMaps_.size(); ++t)
            resolveVsTileMap(entity, body, tf, *tileMaps_[t].first, tileMaps_[t].second);
    }

    // --- Entity vs Entity (each unique pair once) ---
    for (size_t i = 0; i < entities.size(); ++i) {
        RigidBodyComponent& bodyA = entities[i]->getComponent<RigidBodyComponent>();
        for (size_t j = i + 1; j < entities.size(); ++j) {
            RigidBodyComponent& bodyB = entities[j]->getComponent<RigidBodyComponent>();

            if (bodyA.bodyType == Collision::BodyType::Static &&
                bodyB.bodyType == Collision::BodyType::Static) continue;

            if (!(bodyA.mask & bodyB.layer) && !(bodyB.mask & bodyA.layer)) continue;

            resolveVsEntity(
                entities[i], bodyA, entities[i]->getComponent<TransformComponent>(),
                entities[j], bodyB, entities[j]->getComponent<TransformComponent>());
        }
    }
}

// ---------------------------------------------------------------------------
// Debug draw
// ---------------------------------------------------------------------------

void CollisionSystem::draw() {
    if (!Core::renderer) return;

    const int ppu = Core::settings.render.pixelsPerUnit;
    const size_t systemID = SystemID::getSystemTypeID<CollisionSystem>();
    auto& entities = entityManager_->getEntitiesForSystem(systemID);

    for (size_t i = 0; i < entities.size(); ++i) {
        const RigidBodyComponent& body = entities[i]->getComponent<RigidBodyComponent>();
        if (!body.showShape) continue;

        const TransformComponent& tf = entities[i]->getComponent<TransformComponent>();
        const Collision::AABB aabb = worldAABB(body, tf);

        switch (body.bodyType) {
            case Collision::BodyType::Dynamic:
                SDL_SetRenderDrawColor(Core::renderer, 255,  80,  80, 255); break;
            case Collision::BodyType::Kinematic:
                SDL_SetRenderDrawColor(Core::renderer,  80, 180, 255, 255); break;
            case Collision::BodyType::Static:
                SDL_SetRenderDrawColor(Core::renderer,  80, 220,  80, 255); break;
        }

        SDL_FRect rect {
            aabb.x * static_cast<float>(ppu),
            aabb.y * static_cast<float>(ppu),
            aabb.w * static_cast<float>(ppu),
            aabb.h * static_cast<float>(ppu)
        };
        SDL_RenderDrawRectF(Core::renderer, &rect);
    }
}

} // namespace PRL
