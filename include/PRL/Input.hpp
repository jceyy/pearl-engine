#ifndef PRL_INPUT_HPP_INCLUDED
#define PRL_INPUT_HPP_INCLUDED

#include <cstdint>
#include <vector>
#include <type_traits>
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
//! Actions are identified by dense integer IDs — define an enum class on the
//! game side and pass its values directly (any enum type is accepted and cast
//! to its underlying integer). Lookup is a plain array index: no strings, no
//! hashing, nothing on the hot path.
//!
//!     enum class Action : PRL::Input::ActionID { Left, Right, Jump };
//!     Input::bindAction(Action::Jump, { SDL_SCANCODE_SPACE });
//!     if (Input::wasActionPressed(Action::Jump)) ...
//!
//! Usage: call update() exactly once per frame, after the SDL_PollEvent loop
//! (polling pumps fresh keyboard state). Queries are stable for the whole frame.
class Input {
public:
    Input() = delete;

    using ActionID = std::uint32_t;

    //! \brief Snapshot the keyboard. Call once per frame after event polling.
    static void update() noexcept;

    // --- Raw key queries -----------------------------------------------------
    static inline bool isDown(SDL_Scancode key) noexcept      { return current_[key] != 0; }
    static inline bool wasPressed(SDL_Scancode key) noexcept  { return current_[key] != 0 && previous_[key] == 0; }
    static inline bool wasReleased(SDL_Scancode key) noexcept { return current_[key] == 0 && previous_[key] != 0; }

    // --- Action mapping ------------------------------------------------------
    //! \brief Bind (or rebind) an action ID to one or more keys.
    //! IDs should be small and dense (enum values): storage grows to the largest ID.
    static void bindAction(ActionID action, const std::vector<SDL_Scancode>& keys);
    //! \brief Append a key to an existing (or new) action.
    static void addActionKey(ActionID action, SDL_Scancode key);

    static bool isActionDown(ActionID action) noexcept;
    static bool wasActionPressed(ActionID action) noexcept;
    static bool wasActionReleased(ActionID action) noexcept;

    //! \brief -1, 0 or +1 from two opposing actions (e.g. getAxis(Action::Left, Action::Right)).
    static float getAxis(ActionID negativeAction, ActionID positiveAction) noexcept;

    // --- Enum class convenience overloads (zero-cost: static_cast only) ------
    template<typename E, typename std::enable_if<std::is_enum<E>::value, int>::type = 0>
    static void bindAction(E action, const std::vector<SDL_Scancode>& keys) {
        bindAction(static_cast<ActionID>(action), keys);
    }
    template<typename E, typename std::enable_if<std::is_enum<E>::value, int>::type = 0>
    static void addActionKey(E action, SDL_Scancode key) {
        addActionKey(static_cast<ActionID>(action), key);
    }
    template<typename E, typename std::enable_if<std::is_enum<E>::value, int>::type = 0>
    static bool isActionDown(E action) noexcept {
        return isActionDown(static_cast<ActionID>(action));
    }
    template<typename E, typename std::enable_if<std::is_enum<E>::value, int>::type = 0>
    static bool wasActionPressed(E action) noexcept {
        return wasActionPressed(static_cast<ActionID>(action));
    }
    template<typename E, typename std::enable_if<std::is_enum<E>::value, int>::type = 0>
    static bool wasActionReleased(E action) noexcept {
        return wasActionReleased(static_cast<ActionID>(action));
    }
    template<typename E, typename std::enable_if<std::is_enum<E>::value, int>::type = 0>
    static float getAxis(E negativeAction, E positiveAction) noexcept {
        return getAxis(static_cast<ActionID>(negativeAction), static_cast<ActionID>(positiveAction));
    }

private:
    static bool anyKeyDown_(ActionID action, const Uint8* state) noexcept;

    static Uint8 current_[SDL_NUM_SCANCODES];
    static Uint8 previous_[SDL_NUM_SCANCODES];
    //! Key bindings indexed directly by ActionID.
    static std::vector<std::vector<SDL_Scancode>> actions_;
};

} // namespace PRL

#endif // PRL_INPUT_HPP_INCLUDED
