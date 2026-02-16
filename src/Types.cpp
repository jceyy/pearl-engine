#include "Types.hpp"

size_t PRLObject::instanceCount_ = 0;

PRLObject::PRLObject() {
    instanceCount_++;
}

PRLObject::PRLObject(const PRLObject& other) {
    *this = other; 
    instanceCount_++;
}

PRLObject::~PRLObject() {
    instanceCount_--;
}