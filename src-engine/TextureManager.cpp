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

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect* src, SDL_FRect* dst) {
    SDL_RenderCopyF(Game::renderer, texture, src, dst);
}

