#include "Core.hpp"


using namespace PRL;
using namespace std;

// Static member definitions
SDL_Renderer* Core::renderer = nullptr;
Vec2D<int> Core::screenSize(0, 0);
SDL_Window* Core::window_ = nullptr;

// Managers
EntityManager* Core::entityManager_ = nullptr;
AssetManager* Core::assetManager_ = nullptr;
SystemManager* Core::systemManager_ = nullptr;
bool Core::managersInitialized_ = initManagers_(); // important
bool Core::initialized_ = false;

// Time management
uint64_t Core::currentTimeUS_ = 0;
std::chrono::high_resolution_clock::time_point Core::initTime;


// Methods
void Core::init(SDL_Rect windowRect, Uint32 rendererFlags, Uint32 windowFlags, const std::string& windowName) {
    if (initialized_) {
        return;
    }
    // Should already be done before any function call
    if (!managersInitialized_) 
        managersInitialized_ = initManagers_();
    assert(managersInitialized_);

    screenSize.set(windowRect.w, windowRect.h);

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        PRL::Logging::log("SDL subsystems initialized", "PRL::Core::init()");
        window_ = SDL_CreateWindow(windowName.c_str(), windowRect.x, windowRect.y, windowRect.w, windowRect.h, windowFlags);
        if (window_){
            PRL::Logging::log("Window created", "PRL::Core::init()");
        }
        else{
            PRL::Logging::err("Unable to create window : " + std::string(SDL_GetError()), "PRL::Core::init()");
        }
        // SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
        // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        renderer = SDL_CreateRenderer(window_, -1, rendererFlags);
        if (renderer){
            SDL_RendererInfo info;
            SDL_GetRendererInfo(renderer, &info);
            PRL::Logging::log("Renderer created with driver : " + std::string(info.name), "PRL::Core::init()");
        }
        else{
            PRL::Logging::err("Unable to create renderer : " + std::string(SDL_GetError()), "PRL::Core::init()");
        }

        if (TTF_Init() == -1){
            PRL::Logging::err("Unable to initialize SDL_TTF : " + std::string(SDL_GetError()), "PRL::Core::init()");
        }
        initTime = std::chrono::high_resolution_clock::now();
        initialized_ = true;
    }
}

void Core::clean() {
    PRL::Logging::log("Freeing managers...", "PRL::Core::clean()");
    if (systemManager_) {
        delete systemManager_;
        systemManager_ = nullptr;
    }
    if (assetManager_) {
        delete assetManager_;
        assetManager_ = nullptr;
    }
    PRL::Logging::log("Quitting SDL...", "PRL::Core::clean()");
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window_);
    window_ = nullptr;

    SDL_Quit();
    PRL::Logging::log("Engine cleaned", "PRL::Core::clean()");
}

void Core::updateCurrentTime() noexcept {
    currentTimeUS_ = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - initTime
    ).count();
}

bool Core::initManagers_() {
    entityManager_ = new EntityManager();
    assetManager_ = new AssetManager(*entityManager_);
    systemManager_ = new SystemManager(*entityManager_, *assetManager_);
    if (!entityManager_ || !assetManager_ || !systemManager_) {
        Logging::err("Failed to initialize managers", "PRL::Core::initManagers_");
        return false;
    }
    entityManager_->setSystemManager(systemManager_);
    return true;
}