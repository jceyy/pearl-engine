#include <cmath>
#include "TextureManager.hpp"
#include "Game.hpp"
#include "Logging.hpp"

using namespace PRL;

SDL_Texture* TextureManager::loadTexture(const std::string& fileName){
    int w(0), h(0);
    return loadTexture(fileName, w, h);
}

SDL_Texture* TextureManager::loadTexture(const std::string& fileName, int& w, int& h){
    SDL_Surface* tempSurface = IMG_Load(fileName.c_str());
    if (!tempSurface) 
        Logging::err("Error loading image : " + std::string(IMG_GetError()), "TextureManager::loadTexture");
    w = tempSurface->w;
    h = tempSurface->h;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    
    if (!texture)
        Logging::err("Error creating texture", "TextureManager::loadTexture");

    SDL_FreeSurface(tempSurface);
    return texture;
}

SDL_FRect TextureManager::dst_floored_ = {0, 0, 0, 0};
void TextureManager::Draw(SDL_Texture* texture, SDL_Rect* src, SDL_FRect* dst, SDL_RendererFlip flip) {
    dst_floored_ = {floorf(dst->x + 0.001f), floorf(dst->y + 0.0001f), dst->w, dst->h}; 
    SDL_RenderCopyExF(Game::renderer, texture, src, &dst_floored_, 0, nullptr, flip);
}

inline void setPixel(SDL_Surface* surface, SDL_Color* color, int x, int y) {
    Uint32* const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels
                                             + y * surface->pitch
                                             + x * surface->format->BytesPerPixel);
    *target_pixel = (Uint32) ((color->r << 24) + (color->g << 16) +  (color->b << 8) + (color->a));
}

SDL_Texture* TextureManager::generateCircle(int radius, SDL_Color* color) {
    if (color == nullptr) {
        Logging::err("Null color pointer passed to TextureManager::generateCircle", "TextureManager::generateCircle");
        return nullptr;
    }

    SDL_Surface* surface = SDL_CreateRGBSurface(0, 2 * radius, 2 * radius, 32, 0, 0, 0, 0);
    if (surface == nullptr) {
        Logging::err("Error creating surface : " + std::string(SDL_GetError()), "TextureManager::generateCircle");
        return nullptr;
    }

    int x0(radius), y0(radius); 
    int x = radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y) {
        setPixel(surface, color, x0 + x, y0 + y);
        setPixel(surface, color, x0 + y, y0 + x);
        setPixel(surface, color, x0 - y, y0 + x);
        setPixel(surface, color, x0 - x, y0 + y);
        setPixel(surface, color, x0 - x, y0 - y);
        setPixel(surface, color, x0 - y, y0 - x);
        setPixel(surface, color, x0 + y, y0 - x);
        setPixel(surface, color, x0 + x, y0 - y);

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }
        
        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
