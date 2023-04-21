#include "Collision.hpp"
#include "ECS/ColliderComponent.hpp"


bool Collision::AABB(const PRL_Rect& A, const PRL_Rect& B) {
    if (A.x + A.w >= B.x && B.x + B.w >= A.x &&
        A.y + A.h >= B.y && B.y + B.h >= A.y) {
        return true;
    }
    else return false;
}

bool Collision::AABB(const ColliderComponent& A, const ColliderComponent& B){
    if (AABB(A.collider, B.collider)){
        std::cout << A.tag << " hit " << B.tag << std::endl;
        return true;
    }
    else return false;
}
