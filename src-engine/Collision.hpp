#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Types.hpp"

class Collision {
public:
    static bool AABB(const PRL_Rect& recA, const PRL_Rect& recB);
};


#endif // COLLISION_HPP