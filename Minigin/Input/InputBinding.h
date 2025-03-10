#ifndef INPUTBINDING_H
#define INPUTBINDING_H
#include <memory>

#include "InputAction.h"
#include "Command.h"


namespace fovy {
    enum ButtonState {
        Pressed,
        Down,
        Released,
    };


    struct InputBinding {
        ButtonState buttonState{};
        InputAction action{};
        int controllderIdx{};
        std::unique_ptr<BaseCommand> command{nullptr};

        InputBinding() = default;

        InputBinding(ButtonState state, int controllerId, InputAction action, BaseCommand* command)
            : buttonState(state), action(std::move(action)), controllderIdx(controllerId), command(command) {
        }


        bool ExecuteController(ButtonState state, int button) const {
            if (action.HasControllerButton(button) && buttonState == state) {
                command->Execute();
                return true;
            }
            return false;
        }

        bool ExecuteKeyboard(ButtonState state, SDL_Scancode key) const {
            if (action.HasKeyboardKey(key) && buttonState == state) {
                command->Execute();
                return true;
            }
            return false;
        }
    };
}

#endif //INPUTBINDING_H
