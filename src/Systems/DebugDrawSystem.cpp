#include <cmath>
#include <algorithm>
#include "Systems/DebugDrawSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "TileMap.hpp"
#include "Camera.hpp"
#include "Core.hpp"

namespace PRL {

bool DebugDrawSystem::enabled_ = false;
size_t DebugDrawSystem::instanceCount_ = 0;

DebugDrawSystem::DebugDrawSystem(ComponentSignature signature) : System(signature) {
    systemName_ = "DebugDrawSystem";
    instanceCount_++;
}

DebugDrawSystem::~DebugDrawSystem() {
    instanceCount_--;
}

void DebugDrawSystem::draw() {
    if (!enabled_ || !Core::renderer) return;

    const float ppu = static_cast<float>(Core::settings.render.pixelsPerUnit);
    const Camera& camera = Core::getCamera();
    const Vector2D camTopLeft = camera.topLeft();
    const Collision::AABB view = camera.viewAABB();

    // --- Solid tiles of every tilemap registered with the CollisionSystem ---
    if (collisionSystem_) {
        SDL_SetRenderDrawColor(Core::renderer, 80, 220, 80, 255);

        const auto& tileMaps = collisionSystem_->tileMaps();
        for (size_t t = 0; t < tileMaps.size(); ++t) {
            const TileMap& map = *tileMaps[t].first;
            const LayerID solidLayer = tileMaps[t].second;

            const Vec2D<int> tileSize = map.tileSize();
            const Vec2D<int> mapSize  = map.mapSize();
            const float tsX = tileSize.x / ppu;
            const float tsY = tileSize.y / ppu;

            // Only iterate tiles inside the camera view.
            const int tileX0 = std::max(0,             static_cast<int>(std::floor(view.x / tsX)));
            const int tileY0 = std::max(0,             static_cast<int>(std::floor(view.y / tsY)));
            const int tileX1 = std::min(mapSize.x - 1, static_cast<int>(std::floor((view.x + view.w) / tsX)));
            const int tileY1 = std::min(mapSize.y - 1, static_cast<int>(std::floor((view.y + view.h) / tsY)));

            for (int ty = tileY0; ty <= tileY1; ++ty) {
                for (int tx = tileX0; tx <= tileX1; ++tx) {
                    const int chunkX = tx >> TileMap::CHUNK_BIT_SHIFT;
                    const int chunkY = ty >> TileMap::CHUNK_BIT_SHIFT;
                    const int localX = tx & TileMap::CHUNK_TILE_MASK;
                    const int localY = ty & TileMap::CHUNK_TILE_MASK;

                    const TileChunk& chunk = map.getChunk(chunkX, chunkY);
                    if (solidLayer >= static_cast<LayerID>(chunk.tiles.size())) continue;

                    const Tile& tile = chunk.tiles[solidLayer]
                                                  [localY * TileMap::CHUNK_TILE_SIZE + localX];
                    if (tile.ID == Tile::emptyTileID) continue;

                    const SDL_FRect rect {
                        (tx * tsX - camTopLeft.x) * ppu,
                        (ty * tsY - camTopLeft.y) * ppu,
                        tsX * ppu,
                        tsY * ppu
                    };
                    SDL_RenderDrawRectF(Core::renderer, &rect);
                }
            }
        }
    }

    // --- Entity rigid body AABBs, colored by body type ---
    const size_t systemID = SystemID::getSystemTypeID<DebugDrawSystem>();
    auto& entities = entityManager_->getEntitiesForSystem(systemID);

    for (size_t i = 0; i < entities.size(); ++i) {
        const RigidBodyComponent& body = entities[i]->getComponent<RigidBodyComponent>();
        const TransformComponent& tf = entities[i]->getComponent<TransformComponent>();
        const Collision::AABB& local = body.shape.boundingAABB();

        switch (body.bodyType) {
            case Collision::BodyType::Dynamic:
                SDL_SetRenderDrawColor(Core::renderer, 255,  80,  80, 255); break;
            case Collision::BodyType::Kinematic:
                SDL_SetRenderDrawColor(Core::renderer,  80, 180, 255, 255); break;
            case Collision::BodyType::Static:
                SDL_SetRenderDrawColor(Core::renderer, 230, 210,  60, 255); break;
        }

        const SDL_FRect rect {
            (local.x + tf.position.x - camTopLeft.x) * ppu,
            (local.y + tf.position.y - camTopLeft.y) * ppu,
            local.w * ppu,
            local.h * ppu
        };
        SDL_RenderDrawRectF(Core::renderer, &rect);
    }
    SDL_SetRenderDrawColor(Core::renderer, Core::renderColor.r, Core::renderColor.g, Core::renderColor.b, Core::renderColor.a);
}

} // namespace PRL
