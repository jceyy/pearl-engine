#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED

#include <chrono>
#include "SDL2/SDL.h"
#include "AssetManager.hpp"
#include "Systems/Systems.hpp"

namespace PRL {

    class Core;

    class PRLSettings {
    public:
        class Rendering {
        public:
            int pixelsPerUnit = 32;
        
        private:
            Vec2D<int> logicalSize_ = {0,0};
            Vec2D<int> refLogicalSize_ = {0,0};
            float logicalScaleToRef_ = 1.0f;
            Vec2D<int> screenSize_ = {0,0};

            friend class Core;
        };
        Rendering render;
    };


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

        static PRLSettings settings;
        static inline Vec2D<int> getLogicalSize() noexcept { return settings.render.logicalSize_; }
        static inline Vec2D<int> getScreenSize() noexcept { return settings.render.screenSize_; }

    private:
        static SDL_Window* window_;
        static bool initialized_;

        // Time management
        static uint64_t currentTimeUS_;
        static std::chrono::_V2::system_clock::time_point initTime;
    };
}


#endif // CORE_H_INCLUDED