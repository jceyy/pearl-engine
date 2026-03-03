#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED

#include <chrono>
#include "SDL2/SDL.h"
#include "AssetManager.hpp"
#include "Systems/Systems.hpp"

namespace PRL {

    class Core {
    public:
        Core() {}
        Core(const Core& other) = delete;
        Core(Core&& other) noexcept = delete;
        Core& operator=(const Core& other) = delete;
        Core& operator=(Core&& other) noexcept = delete;
        ~Core() = delete;

        static void init(SDL_Rect windowRect, Uint32 rendererFlags = (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC), Uint32 windowFlags = 0, const std::string& windowName = "pearl-engine alpha");
        static void clean();
        static inline bool isInitialized() noexcept { return initialized_; } 

        static inline AssetManager& getAssetManager() noexcept {
            static AssetManager instance(getEntityManager());
            return instance;
        }

        static inline EntityManager& getEntityManager() noexcept {
            static EntityManager instance;
            return instance;
        }

        static inline SystemManager& getSystemManager() noexcept {
            static SystemManager instance(getEntityManager(), getAssetManager());
            static bool initialized = false;
            if (!initialized) {
                getEntityManager().setSystemManager(&instance);
                initialized = true;
            }
            return instance;
        }
    
        static void updateCurrentTime() noexcept;
        inline static uint64_t getCurrentTimeUS() noexcept { return currentTimeUS_; }
        static SDL_Renderer* renderer;
        static Vec2D<int> screenSize;

    private:
        static SDL_Window* window_;
        static bool initialized_;

        // Time management
        static uint64_t currentTimeUS_;
        static std::chrono::_V2::system_clock::time_point initTime;
    };
}


#endif // CORE_H_INCLUDED