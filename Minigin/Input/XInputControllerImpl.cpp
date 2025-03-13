#ifndef XINPUTCONTROLLERIMPL_H
#define XINPUTCONTROLLERIMPL_H
#include "InputManager.h"

#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <windows.h>
#include <XInput.h>

namespace fovy {
    class fovy::InputManager::ControllerImpl {
    public:
        void HandleController(const std::vector<InputBinding>& binds) {
            UpdateControllerInfo();

            for (auto& bind: binds) {
                for (auto& button: bind.action.controllerButtons) {
                    if (bind.buttonState == ButtonState::Down && IsDown(static_cast<WORD>(button), bind.controllderIdx)) {
                        bind.command->Execute();
                        break;
                    } else {
                        if (bind.buttonState == ButtonState::Pressed && IsDownThisFrame(static_cast<WORD>(button), bind.controllderIdx)) {
                            bind.command->Execute();
                        } else if (bind.buttonState == ButtonState::Released && IsUpThisFrame(static_cast<WORD>(button), bind.controllderIdx)) {
                            bind.command->Execute();
                        }
                    }
                }
            }
        }

        bool IsDownThisFrame(WORD button, int controllerId = 0) const {
            if (m_ControllerStates.size() > 0 && m_ControllerStates.size() > controllerId) {
                return m_ControllerStates[controllerId].buttonsPressed & button;
            }
            return false;
        }

        bool IsUpThisFrame(WORD button, int controllerId = 0) const {
            if (m_ControllerStates.size() > 0 && m_ControllerStates.size() > controllerId) {
                return m_ControllerStates[controllerId].buttonsReleased & button;
            }
            return false;
        }

        bool IsHeld(WORD button, int controllerId = 0) const {
            if (m_ControllerStates.size() > 0  && m_ControllerStates.size() > controllerId) {
                return m_ControllerStates[controllerId].currControlerState.Gamepad.wButtons & button;
            }
            return false;
        }

        bool IsDown(WORD button, int controllerId = 0) const {
            if (m_ControllerStates.size() > 0) {
                return IsDownThisFrame(button, controllerId) || IsHeld(button, controllerId);
            }
            return false;
        }

    private:
        struct XINPUT_CONTROLLER_STATE {
            XINPUT_STATE currControlerState;
            XINPUT_STATE prevControllerState;
            WORD buttonsPressed;
            WORD buttonsReleased;
        };


        void UpdateControllerInfo() {
            DWORD dwResult;
            int joystickCount = 0;

            for (DWORD i = 0; i < XUSER_MAX_COUNT; i++) {
                XINPUT_STATE state;
                ZeroMemory(&state, sizeof(XINPUT_STATE));
                dwResult = XInputGetState(i, &state);
                if (dwResult == ERROR_SUCCESS) {
                    joystickCount++;
                }
            }

            m_ControllerStates.resize(joystickCount);

            for (int controllerIndex = 0; controllerIndex < joystickCount; controllerIndex++) {
                XINPUT_CONTROLLER_STATE& state = m_ControllerStates[controllerIndex];

                state.prevControllerState = state.currControlerState;

                state.currControlerState = {};
                XInputGetState(controllerIndex, &state.currControlerState);

                WORD changes = state.currControlerState.Gamepad.wButtons ^ state.prevControllerState.Gamepad.wButtons;
                state.buttonsPressed = changes & state.currControlerState.Gamepad.wButtons;
                state.buttonsReleased = changes & (~state.currControlerState.Gamepad.wButtons);
            }
        }


        std::vector<XINPUT_CONTROLLER_STATE> m_ControllerStates;
    };

    bool fovy::InputManager::IsControllerButtonDown(int button) const {
        return m_pImpl->IsDown(static_cast<WORD>(button));
    }

    bool fovy::InputManager::IsControllerButtonUp(int button) const {
        return m_pImpl->IsUpThisFrame(static_cast<WORD>(button));
    }

    bool fovy::InputManager::IsControllerButtonPressed(int button) const {
        return m_pImpl->IsDownThisFrame(static_cast<WORD>(button));
    }

    bool fovy::InputManager::IsControllerButtonReleased(int button) const {
        return m_pImpl->IsUpThisFrame(static_cast<WORD>(button));
    }

    void fovy::InputManager::HandleControllerInput() {
        return m_pImpl->HandleController(m_Bindings);
    }

    fovy::InputManager::InputManager() {
        m_pImpl = std::make_unique<ControllerImpl>();
    }

    fovy::InputManager::~InputManager() = default;
};


#endif //XINPUTCONTROLLERIMPL_H
