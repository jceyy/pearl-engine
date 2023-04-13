#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

class TextureManager{

public:
    virtual ~TextureManager() = 0;

    static SDL_Texture* loadTexture(const std::string& fileName);
    static SDL_Texture* loadTexture(const std::string& fileName, int& w, int& h);
    static void Draw(SDL_Texture* texture, SDL_Rect* src, SDL_FRect* dst);
};


#endif // TEXTURE_MANAGER_HPP