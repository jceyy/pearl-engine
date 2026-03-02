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

        inline static AssetManager& getAssetManager() noexcept { return *assetManager_; }
        inline static SystemManager& getSystemManager() noexcept { return *systemManager_; }
        inline static EntityManager& getEntityManager() noexcept { return *entityManager_; }
    
        static void updateCurrentTime() noexcept;
        inline static uint64_t getCurrentTimeUS() noexcept { return currentTimeUS_; }
        static SDL_Renderer* renderer;
        static Vec2D<int> screenSize;

    private:
        static bool initManagers_(); 

        static SDL_Window* window_;

        // Managers
        static EntityManager* entityManager_;
        static AssetManager* assetManager_;
        static SystemManager* systemManager_;
        static bool managersInitialized_;
        static bool initialized_;

        // Time management
        static uint64_t currentTimeUS_;
        static std::chrono::_V2::system_clock::time_point initTime;
    };
}


#endif // CORE_H_INCLUDED