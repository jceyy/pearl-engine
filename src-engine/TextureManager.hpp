#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

class TextureManager{

public:
    TextureManager();
    ~TextureManager();

    static SDL_Texture* loadTexture(const std::string& fileName, SDL_Renderer* renderer);
};


#endif // TEXTURE_MANAGER_HPP