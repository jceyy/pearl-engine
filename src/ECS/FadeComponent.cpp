#include "Core.hpp"
#include "ECS/FadeComponent.hpp"

FadeComponent::FadeComponent() {}

FadeComponent::~FadeComponent() {}

void FadeComponent::update() {
    currentTime_us_ = PRL::Core::getCurrentTimeUS();

    for (int n(fadingGroup_.size() - 1); n >= 0 ; --n) {
        if (currentTime_us_ - fadingGroup_[n]->init_time_ > fadingGroup_[n]->duration_us_) {
            fadingGroup_[n]->setFinal_();
            // Free memory since the fading is complete
            delete fadingGroup_[n];
            // Remove from the fadingGroup_ vector
            fadingGroup_.erase(fadingGroup_.begin() + n);
        }
        else {
            fadingGroup_[n]->update_(currentTime_us_);
        }
    }
}