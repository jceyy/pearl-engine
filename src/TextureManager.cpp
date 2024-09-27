#include "TextureManager.hpp"
#include "Game.hpp"

SDL_Texture* TextureManager::loadTexture(const std::string& fileName){
    int w(0), h(0);
    return loadTexture(fileName, w, h);
}


SDL_Texture* TextureManager::loadTexture(const std::string& fileName, int& w, int& h){
    SDL_Surface* tempSurface = IMG_Load(fileName.c_str());
    if (!tempSurface) std::cerr << "Error loading image" << std::endl;
    w = tempSurface->w;
    h = tempSurface->h;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    
    if (!texture) std::cerr << "Error creating texture" << std::endl;

    SDL_FreeSurface(tempSurface);
    return texture;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect* src, SDL_FRect* dst, SDL_RendererFlip flip) {
    SDL_RenderCopyExF(Game::renderer, texture, src, dst, 0, nullptr, flip);
}

inline void setPixel(SDL_Surface* surface, SDL_Color* color, int x, int y) {
    Uint32* const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels
                                             + y * surface->pitch
                                             + x * surface->format->BytesPerPixel);
    *target_pixel = (Uint32) ((color->r << 24) + (color->g << 16) +  (color->b << 8) + (color->a));
}

SDL_Texture* TextureManager::generateCircle(int radius, SDL_Color* color) {

    SDL_Surface* surface = SDL_CreateRGBSurface(0, 2 * radius, 2 * radius, 32, 0, 0, 0, 0);
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
