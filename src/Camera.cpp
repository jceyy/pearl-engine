#include <cmath>
#include "Camera.hpp"
#include "Core.hpp"

namespace PRL {

size_t Camera::instanceCount_ = 0;

Camera::Camera() :
position_(0.f, 0.f), viewport_(0.f, 0.f), target_(0.f, 0.f), deadZone_(0.f, 0.f),
smoothing_(0.f), bounds_(0.f, 0.f, 0.f, 0.f), hasTarget_(false), hasBounds_(false) {
    instanceCount_++;
}

Camera::~Camera() {
    instanceCount_--;
}

// --- View -------------------------------------------------------------------

void Camera::setViewportSize(float w, float h) noexcept {
    viewport_.set(w, h);
}

void Camera::setPosition(float x, float y) noexcept {
    position_.set(x, y);
    clampToBounds_(position_);
}

// --- Following --------------------------------------------------------------

void Camera::setTarget(const Vector2D& worldPos) noexcept {
    target_ = worldPos;
    hasTarget_ = true;
}

void Camera::clearTarget() noexcept {
    hasTarget_ = false;
}

void Camera::setDeadZone(float halfWidth, float halfHeight) noexcept {
    deadZone_.set(halfWidth > 0.f ? halfWidth : 0.f,
                  halfHeight > 0.f ? halfHeight : 0.f);
}

void Camera::setSmoothing(float sharpness) noexcept {
    smoothing_ = sharpness > 0.f ? sharpness : 0.f;
}

void Camera::snapToTarget() noexcept {
    if (!hasTarget_) return;
    position_ = desiredPosition_();
}

// --- Bounds -----------------------------------------------------------------

void Camera::setBounds(const Collision::AABB& worldBounds) noexcept {
    bounds_ = worldBounds;
    hasBounds_ = true;
    clampToBounds_(position_);
}

void Camera::clearBounds() noexcept {
    hasBounds_ = false;
}

// --- Update -----------------------------------------------------------------

Vector2D Camera::desiredPosition_() const noexcept {
    // Only pull the camera when the target leaves the dead zone box around the center.
    Vector2D desired = position_;
    const float dx = target_.x - position_.x;
    const float dy = target_.y - position_.y;

    if (dx >  deadZone_.x) desired.x = target_.x - deadZone_.x;
    if (dx < -deadZone_.x) desired.x = target_.x + deadZone_.x;
    if (dy >  deadZone_.y) desired.y = target_.y - deadZone_.y;
    if (dy < -deadZone_.y) desired.y = target_.y + deadZone_.y;

    clampToBounds_(desired);
    return desired;
}

void Camera::clampToBounds_(Vector2D& center) const noexcept {
    if (!hasBounds_) return;

    const float halfW = viewport_.x * 0.5f;
    const float halfH = viewport_.y * 0.5f;

    // Per axis: clamp the view inside the bounds; if the bounds are smaller
    // than the viewport, center the view on them instead.
    if (bounds_.w >= viewport_.x) {
        if (center.x - halfW < bounds_.x)             center.x = bounds_.x + halfW;
        if (center.x + halfW > bounds_.x + bounds_.w) center.x = bounds_.x + bounds_.w - halfW;
    } else {
        center.x = bounds_.centerX();
    }

    if (bounds_.h >= viewport_.y) {
        if (center.y - halfH < bounds_.y)             center.y = bounds_.y + halfH;
        if (center.y + halfH > bounds_.y + bounds_.h) center.y = bounds_.y + bounds_.h - halfH;
    } else {
        center.y = bounds_.centerY();
    }
}

void Camera::update(double dt) noexcept {
    if (hasTarget_) {
        const Vector2D desired = desiredPosition_();
        if (smoothing_ <= 0.f) {
            position_ = desired;
        } else {
            // Exponential, framerate-independent approach towards the desired position.
            const float alpha = 1.f - std::exp(-smoothing_ * static_cast<float>(dt));
            position_.x += (desired.x - position_.x) * alpha;
            position_.y += (desired.y - position_.y) * alpha;
        }
    }
    clampToBounds_(position_);
}

// --- Coordinate transforms --------------------------------------------------

Vector2D Camera::worldToScreen(const Vector2D& world) const noexcept {
    const float ppu = static_cast<float>(Core::settings.render.pixelsPerUnit);
    const Vector2D tl = topLeft();
    return Vector2D((world.x - tl.x) * ppu, (world.y - tl.y) * ppu);
}

Vector2D Camera::screenToWorld(const Vector2D& screen) const noexcept {
    const float ppu = static_cast<float>(Core::settings.render.pixelsPerUnit);
    const Vector2D tl = topLeft();
    return Vector2D(screen.x / ppu + tl.x, screen.y / ppu + tl.y);
}

} // namespace PRL
