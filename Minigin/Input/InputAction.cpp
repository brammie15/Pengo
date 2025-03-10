#include "InputAction.h"

#include <algorithm>

bool fovy::InputAction::HasKeyboardKey(SDL_Scancode compareKey) const {
    return std::ranges::count(keyboardButtons,compareKey) > 0;
}

bool fovy::InputAction::HasControllerButton(int compareButton) const {
    return std::ranges::count(controllerButtons,compareButton) > 0;
}
