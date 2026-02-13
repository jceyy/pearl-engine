#ifndef TYPES_HPP
#define TYPES_HPP

#include <limits>
#include <SDL2/SDL.h>

using PosType = float;
// using PRL_Rect = SDL_FRect;


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

#endif // TYPES_HPP