#ifndef INPUTACTION_H
#define INPUTACTION_H
#include <vector>
#include <SDL.h>

namespace fovy {
    struct InputAction
    {
        std::vector<SDL_Scancode> keyboardButtons;
        std::vector<int> controllerButtons;
        // std::vector<SDL_GameControllerAxis> controllerAxis; TODO:

        bool HasKeyboardKey(SDL_Scancode compareKey) const;
        bool HasControllerButton(int compareButton) const;
    };
}

#endif //INPUTACTION_H
