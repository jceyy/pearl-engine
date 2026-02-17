#ifndef TYPES_HPP_INCLUDED
#define TYPES_HPP_INCLUDED

#include <limits>
#include <SDL2/SDL.h>

using PosType = float;
using PRL_Rect = SDL_FRect; // to be replaced ?


#include "Vector2D.hpp"

const float INFINITY_F(std::numeric_limits<float>::infinity());

// Generic PRL object class
class PRLObject {
public:
    PRLObject();
    PRLObject(const PRLObject& other);
    PRLObject& operator=(const PRLObject& other) = default;
    PRLObject(PRLObject&& other) noexcept = default;
    PRLObject& operator=(PRLObject&& other) noexcept = default;
    virtual ~PRLObject() = 0;

    static inline size_t instanceCount() noexcept { return instanceCount_; }

private:
    static size_t instanceCount_;
};

using TextureID = std::string;
using AnimationID = std::string;

#endif // TYPES_HPP_INCLUDED