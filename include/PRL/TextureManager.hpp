#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <string>
#include <vector>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "AssetManager.hpp"


class TextureManager{
public:
    virtual ~TextureManager() = 0;

    static SDL_Texture* loadTexture(const std::string& fileName);
    static SDL_Texture* loadTexture(const std::string& fileName, int& w, int& h);
    static std::vector<SpriteRegion> loadSpriteRegions(const std::string& fileName);

    static void Draw(SDL_Texture* texture, SDL_Rect* src, SDL_FRect* dst, SDL_RendererFlip flip, double angle = 0.0);

    static SDL_Texture* generateCircle(int radius, SDL_Color* color);

private:
    static SDL_FRect dst_floored_;
};


#endif // TEXTURE_MANAGER_HPP