#include <cstring>
#include <cassert>
#include "Input.hpp"

namespace PRL {

Uint8 Input::current_[SDL_NUM_SCANCODES] = {0};
Uint8 Input::previous_[SDL_NUM_SCANCODES] = {0};
std::vector<std::vector<SDL_Scancode>> Input::actions_;

void Input::update() noexcept {
    std::memcpy(previous_, current_, SDL_NUM_SCANCODES);
    int numKeys = 0;
    const Uint8* state = SDL_GetKeyboardState(&numKeys);
    if (numKeys > SDL_NUM_SCANCODES) numKeys = SDL_NUM_SCANCODES;
    std::memcpy(current_, state, numKeys);
}

// --- Action mapping ----------------------------------------------------------

void Input::bindAction(ActionID action, const std::vector<SDL_Scancode>& keys) {
    if (action >= actions_.size()) actions_.resize(action + 1);
    actions_[action] = keys;
}

void Input::addActionKey(ActionID action, SDL_Scancode key) {
    if (action >= actions_.size()) actions_.resize(action + 1);
    actions_[action].push_back(key);
}

bool Input::anyKeyDown_(ActionID action, const Uint8* state) noexcept {
    assert(action < actions_.size() && "Input action queried before being bound");
    if (action >= actions_.size()) return false;

    const std::vector<SDL_Scancode>& keys = actions_[action];
    for (size_t i = 0; i < keys.size(); ++i)
        if (state[keys[i]] != 0) return true;
    return false;
}

bool Input::isActionDown(ActionID action) noexcept {
    return anyKeyDown_(action, current_);
}

bool Input::wasActionPressed(ActionID action) noexcept {
    return anyKeyDown_(action, current_) && !anyKeyDown_(action, previous_);
}

bool Input::wasActionReleased(ActionID action) noexcept {
    return !anyKeyDown_(action, current_) && anyKeyDown_(action, previous_);
}

float Input::getAxis(ActionID negativeAction, ActionID positiveAction) noexcept {
    float axis = 0.f;
    if (isActionDown(negativeAction)) axis -= 1.f;
    if (isActionDown(positiveAction)) axis += 1.f;
    return axis;
}

} // namespace PRL
