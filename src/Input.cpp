#include <cstring>
#include "Input.hpp"
#include "Logging.hpp"

namespace PRL {

Uint8 Input::current_[SDL_NUM_SCANCODES] = {0};
Uint8 Input::previous_[SDL_NUM_SCANCODES] = {0};
std::unordered_map<std::string, std::vector<SDL_Scancode>> Input::actions_;

void Input::update() noexcept {
    std::memcpy(previous_, current_, SDL_NUM_SCANCODES);
    int numKeys = 0;
    const Uint8* state = SDL_GetKeyboardState(&numKeys);
    if (numKeys > SDL_NUM_SCANCODES) numKeys = SDL_NUM_SCANCODES;
    std::memcpy(current_, state, numKeys);
}

// --- Raw key queries ---------------------------------------------------------

bool Input::isDown(SDL_Scancode key) noexcept {
    return current_[key] != 0;
}

bool Input::wasPressed(SDL_Scancode key) noexcept {
    return current_[key] != 0 && previous_[key] == 0;
}

bool Input::wasReleased(SDL_Scancode key) noexcept {
    return current_[key] == 0 && previous_[key] != 0;
}

// --- Action mapping ----------------------------------------------------------

void Input::bindAction(const std::string& action, const std::vector<SDL_Scancode>& keys) {
    actions_[action] = keys;
}

void Input::addActionKey(const std::string& action, SDL_Scancode key) {
    actions_[action].push_back(key);
}

bool Input::anyKeyDown_(const std::string& action, const Uint8* state) noexcept {
    auto it = actions_.find(action);
    if (it == actions_.end()) {
        Logging::err("Unknown input action '" + action + "'", "Input");
        return false;
    }
    for (SDL_Scancode key : it->second)
        if (state[key] != 0) return true;
    return false;
}

bool Input::isActionDown(const std::string& action) noexcept {
    return anyKeyDown_(action, current_);
}

bool Input::wasActionPressed(const std::string& action) noexcept {
    return anyKeyDown_(action, current_) && !anyKeyDown_(action, previous_);
}

bool Input::wasActionReleased(const std::string& action) noexcept {
    return !anyKeyDown_(action, current_) && anyKeyDown_(action, previous_);
}

float Input::getAxis(const std::string& negativeAction, const std::string& positiveAction) noexcept {
    float axis = 0.f;
    if (isActionDown(negativeAction)) axis -= 1.f;
    if (isActionDown(positiveAction)) axis += 1.f;
    return axis;
}

} // namespace PRL
