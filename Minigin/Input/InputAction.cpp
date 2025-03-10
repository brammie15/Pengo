#include "InputAction.h"

bool InputAction::HasKeyboardKey(SDL_Scancode compareKey) const {
    return std::ranges::count(keyboardButtons,compareKey) > 0;
}

bool InputAction::HasControllerButton(int compareButton) const {
    return std::ranges::count(controllerButtons,compareButton) > 0;
}
