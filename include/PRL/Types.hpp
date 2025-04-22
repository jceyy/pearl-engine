#ifndef TYPES_HPP
#define TYPES_HPP

#include <limits>
#include <SDL2/SDL.h>

using PosType = float;
// using PRL_Rect = struct PRL_Rect{ float x, y, w, h; };
using PRL_Rect = SDL_FRect;


#include "Vector2D.hpp"

const float INFINITY_F(std::numeric_limits<float>::infinity());

#endif // TYPES_HPP