#include "Collision.hpp"



bool Collision::AABB(const PRL_Rect& A, const PRL_Rect& B) {
    if (A.x + A.w >= B.x && B.x + B.w >= A.x &&
        A.y + A.h >= B.y && B.y + B.h >= A.y) {
        return true;
    }
    else return false;
}