#ifndef _CAMERA_INCLUDED_HPP
#define _CAMERA_INCLUDED_HPP

#include "Vector2D.hpp"

namespace PRL{

    class Camera{
    public:
        Camera(); 
        ~Camera();

        static size_t getInstanceCount() noexcept { return instanceCount_; }
    
        private:
        static size_t instanceCount_;
    };
}

#endif // _CAMERA_INCLUDED_HPP