#ifndef INPUTACTION_H
#define INPUTACTION_H
#include <vector>
#include <SDL.h>

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
    };
}

#endif //INPUTACTION_H
