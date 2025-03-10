#ifndef INPUTACTION_H
#define INPUTACTION_H
#include <algorithm>
#include <vector>
#include <SDL.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>


struct InputAction
{
    std::vector<SDL_Scancode> keyboardButtons;
    std::vector<int> controllerButtons;
    // std::vector<SDL_GameControllerAxis> controllerAxis; TODO:

    bool HasKeyboardKey(SDL_Scancode compareKey) const;
    bool HasControllerButton(int compareButton) const;
};


#endif //INPUTACTION_H
