#ifndef _CAMERA_INCLUDED_HPP
#define _CAMERA_INCLUDED_HPP

#include "Vector2D.hpp"
#include "Collision.hpp"

namespace PRL{

    //! \brief 2D camera working in world units. 'position' is the center of the view.
    //!
    //! The camera owns no rendering state: render systems query topLeft() /
    //! worldToScreen() to offset their draw calls. A default "main" camera is
    //! owned by Core (Core::getCamera()) and its viewport is initialized from
    //! the logical size and pixelsPerUnit in Core::init().
    class Camera{
    public:
        Camera();
        Camera(const Camera& other) = default;
        ~Camera();

        // --- View -----------------------------------------------------------
        //! \brief Viewport size in world units.
        void setViewportSize(float w, float h) noexcept;
        inline Vector2D viewportSize() const noexcept { return viewport_; }

        //! \brief Center of the view in world units. Snaps immediately (no smoothing).
        void setPosition(float x, float y) noexcept;
        inline const Vector2D& position() const noexcept { return position_; }

        //! \brief World coordinates of the top-left corner of the view.
        inline Vector2D topLeft() const noexcept {
            return Vector2D(position_.x - viewport_.x * 0.5f,
                            position_.y - viewport_.y * 0.5f);
        }

        //! \brief The visible world-space rectangle.
        inline Collision::AABB viewAABB() const noexcept {
            const Vector2D tl = topLeft();
            return Collision::AABB(tl.x, tl.y, viewport_.x, viewport_.y);
        }

        // --- Following ------------------------------------------------------
        //! \brief Set the world position to follow. Call every update (e.g. player center).
        void setTarget(const Vector2D& worldPos) noexcept;
        void clearTarget() noexcept;

        //! \brief Half-extents (world units) of a box around the camera center inside
        //! which the target can move without dragging the camera. (0,0) = always centered.
        void setDeadZone(float halfWidth, float halfHeight) noexcept;

        //! \brief Exponential follow sharpness in 1/s. 0 = snap instantly.
        //! Typical values: 5 (loose) to 15 (tight). Framerate independent.
        void setSmoothing(float sharpness) noexcept;

        //! \brief Move immediately to the target (ignoring smoothing), e.g. after a level load.
        void snapToTarget() noexcept;

        // --- Bounds ---------------------------------------------------------
        //! \brief Confine the view to a world-space rectangle (e.g. the tilemap extents).
        //! If the bounds are smaller than the viewport on an axis, the view is centered on it.
        void setBounds(const Collision::AABB& worldBounds) noexcept;
        void clearBounds() noexcept;

        //! \brief Advance following/smoothing by dt seconds and clamp to bounds.
        void update(double dt) noexcept;

        // --- Coordinate transforms -----------------------------------------
        //! \brief World units -> screen pixels (uses Core::settings.render.pixelsPerUnit).
        Vector2D worldToScreen(const Vector2D& world) const noexcept;
        //! \brief Screen pixels -> world units.
        Vector2D screenToWorld(const Vector2D& screen) const noexcept;

        static size_t getInstanceCount() noexcept { return instanceCount_; }

    private:
        //! \brief Where the camera center wants to be, honoring dead zone and bounds.
        Vector2D desiredPosition_() const noexcept;
        void clampToBounds_(Vector2D& center) const noexcept;

        Vector2D position_;
        Vector2D viewport_;
        Vector2D target_;
        Vector2D deadZone_;   // half extents
        float smoothing_;
        Collision::AABB bounds_;
        bool hasTarget_;
        bool hasBounds_;

        static size_t instanceCount_;
    };
}

#endif // _CAMERA_INCLUDED_HPP
