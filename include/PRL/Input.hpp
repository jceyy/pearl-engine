#ifndef PRL_INPUT_HPP_INCLUDED
#define PRL_INPUT_HPP_INCLUDED

#include <string>
#include <vector>
#include <unordered_map>
#include <SDL2/SDL.h>

namespace PRL {

//! \brief Frame-based keyboard input with edge detection and action mapping.
//!
//! Edge detection = comparing this frame's key state with the previous frame's:
//!   - isDown():      the key is held right now (level; true every frame while held)
//!   - wasPressed():  the key went up -> down THIS frame (edge; true exactly once)
//!   - wasReleased(): the key went down -> up THIS frame (edge; true exactly once)
//! Jumping, toggles and menu navigation need edges; running needs levels.
//!
//! Actions decouple gameplay code from physical keys: bind "jump" to SPACE (and
//! later a gamepad button), then query the action. Edge queries are evaluated at
//! the action level, so holding one bound key while pressing another does not
//! re-trigger the action.
//!
//! Usage: call update() exactly once per frame, after the SDL_PollEvent loop
//! (polling pumps fresh keyboard state). Queries are stable for the whole frame.
class Input {
public:
    Input() = delete;

    //! \brief Snapshot the keyboard. Call once per frame after event polling.
    static void update() noexcept;

    // --- Raw key queries -----------------------------------------------------
    static bool isDown(SDL_Scancode key) noexcept;
    static bool wasPressed(SDL_Scancode key) noexcept;
    static bool wasReleased(SDL_Scancode key) noexcept;

    // --- Action mapping ------------------------------------------------------
    //! \brief Bind (or rebind) an action name to one or more keys.
    static void bindAction(const std::string& action, const std::vector<SDL_Scancode>& keys);
    //! \brief Append a key to an existing (or new) action.
    static void addActionKey(const std::string& action, SDL_Scancode key);

    static bool isActionDown(const std::string& action) noexcept;
    static bool wasActionPressed(const std::string& action) noexcept;
    static bool wasActionReleased(const std::string& action) noexcept;

    //! \brief -1, 0 or +1 from two opposing actions (e.g. getAxis("left", "right")).
    static float getAxis(const std::string& negativeAction, const std::string& positiveAction) noexcept;

private:
    static bool anyKeyDown_(const std::string& action, const Uint8* state) noexcept;

    static Uint8 current_[SDL_NUM_SCANCODES];
    static Uint8 previous_[SDL_NUM_SCANCODES];
    static std::unordered_map<std::string, std::vector<SDL_Scancode>> actions_;
};

} // namespace PRL

#endif // PRL_INPUT_HPP_INCLUDED
