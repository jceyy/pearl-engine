#ifndef _FADE_COMPONENT_HPP_INCLUDED
#define _FADE_COMPONENT_HPP_INCLUDED

#include <type_traits>
#include "ECS.hpp"

class FadeComponent;
class FadingGroupBase {
public:
    FadingGroupBase(float init, float final, Uint64 duration_us, Uint64 init_time) :
    init_value_(init), final_value_(final), duration_us_(duration_us), init_time_(init_time) {
        coefficient_ = static_cast<float>((final_value_ - init_value_) / duration_us_);
    }
    virtual ~FadingGroupBase() = 0;

private:
    float init_value_;
    float final_value_;
    Uint64 duration_us_;
    Uint64 init_time_;
    float coefficient_;

    virtual void update_(Uint64 currentTime_us) {currentTime_us+=1;}
    virtual void setFinal_() {}

    friend class FadeComponent;
};

template <typename T> class FadingGroup : public FadingGroupBase {
public:
    FadingGroup(const T* data, float init, float final, Uint64 duration_us, Uint64 init_time) :
    FadingGroupBase(init, final, duration_us, init_time),
    variable_(data) {
        static_assert(std::is_arithmetic<T>::value, "Type is not of an arithmetic type");
    }
    ~FadingGroup() {};

private:
    const   T* variable_;
    
    // performs no checks as a design choice for efficiency. Checks done in FadeComponent
    inline void update_(Uint64 currentTime_us) override {
        *variable_ = static_cast<T>(coefficient_ * (currentTime_us - init_time_) + init_value_); 
    }

    inline void setFinal() override {*variable_ = static_cast<T>(final_value_); }
};

class FadeComponent : public Component {
public:
    FadeComponent();
    ~FadeComponent();

    void update() override;
    template <typename T> inline void fade(const T* variable, float init, float final, Uint64 duration_us) {
        fadingGroup_.push_back(new FadingGroup<T>(variable, init, final, duration_us));
    };

    template <typename T> inline void stopFade(const T* var) {
        for (size_t n(0); n < fadingGroup_.size(); ++n) {
            if (static_cast<FadingGroup<T>*>(fadingGroup_[n])->variable_ == var) {
                fadingGroup_.erase(fadingGroup_.begin() + n);
                return;
            }
        }
    }

private:
    std::vector<FadingGroupBase*> fadingGroup_;
    Uint64 currentTime_us_;
};

#endif // _FADE_COMPONENT_HPP_INCLUDED