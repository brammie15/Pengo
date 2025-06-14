#ifndef INPUTACTION_H
#define INPUTACTION_H
#include <SDL.h>
#include <vector>

namespace fovy {
    struct AxisBinding {
        SDL_GameControllerAxis axis{};
        float threshold{}; // e.g., 0.5 for right on left stick
        bool positive{};    // true = right/down, false = left/up
    };

    struct InputAction
    {
        std::vector<SDL_Scancode> keyboardButtons;
        std::vector<SDL_GameControllerButton> controllerButtons;
        std::vector<AxisBinding> controllerAxis;

        bool HasKeyboardKey(SDL_Scancode compareKey) const;
        bool HasControllerButton(int compareButton) const;
        bool HasAxis(SDL_GameControllerAxis axis, bool positive) const;
        bool operator==(const InputAction& action) const {
            //Check if both actions have the same keyboard buttons
            if (keyboardButtons.size() != action.keyboardButtons.size()) {
                return false;
            }
            for (size_t i = 0; i < keyboardButtons.size(); ++i) {
                if (keyboardButtons[i] != action.keyboardButtons[i]) {
                    return false;
                }
            }

            //Check if both actions have the same controller buttons
            if (controllerButtons.size() != action.controllerButtons.size()) {
                return false;
            }
            for (size_t i = 0; i < controllerButtons.size(); ++i) {
                if (controllerButtons[i] != action.controllerButtons[i]) {
                    return false;
                }
            }

            return true;
        }
    };
}

#endif //INPUTACTION_H
