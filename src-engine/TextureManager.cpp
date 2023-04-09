#include "TextureManager.hpp"

SDL_Texture* TextureManager::loadTexture(const std::string& fileName, SDL_Renderer* renderer){
    SDL_Surface* tempSurface = IMG_Load(fileName.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return texture;
}