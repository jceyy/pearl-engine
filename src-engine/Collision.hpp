#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Types.hpp"
class ColliderComponent;

class Collision {
public:
    static bool AABB(const PRL_Rect& recA, const PRL_Rect& recB);
    static bool AABB(const ColliderComponent& A, const ColliderComponent& B);
};


#endif // COLLISION_HPP