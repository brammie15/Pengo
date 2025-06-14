#ifndef SDLCONTROLLERIMPL_H
#define SDLCONTROLLERIMPL_H

#include "InputManager.h"
#include <SDL.h>
#include <vector>
#include <unordered_map>
#include <iostream>

namespace fovy {
    class fovy::InputManager::ControllerImpl {
    public:
        ControllerImpl() {
            if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0) {
                std::cerr << "SDL_INIT_GAMECONTROLLER failed: " << SDL_GetError() << std::endl;
            }

            for (int i = 0; i < SDL_NumJoysticks(); ++i) {
                if (SDL_IsGameController(i)) {
                    SDL_GameController* controller = SDL_GameControllerOpen(i);
                    if (controller) {
                        int id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
                        m_Controllers[id] = controller;
                        m_ControllerStates[id] = {};
                    }
                }
            }
        }

        ~ControllerImpl() {
            for (auto& pair : m_Controllers) {
                SDL_GameControllerClose(pair.second);
            }
            m_Controllers.clear();
        }

        void HandleController(const std::vector<InputBinding>& binds) {
            UpdateControllerInfo();

            for (const auto& bind : binds) {
                for (const auto& button : bind.action.controllerButtons) {
                    if (bind.buttonState == ButtonState::Down && IsDown(button, bind.controllderIdx)) {
                        bind.command->Execute();
                        break;
                    } else if (bind.buttonState == ButtonState::Pressed && IsDownThisFrame(button, bind.controllderIdx)) {
                        bind.command->Execute();
                    } else if (bind.buttonState == ButtonState::Released && IsUpThisFrame(button, bind.controllderIdx)) {
                        bind.command->Execute();
                    }
                }
            }
        }

        bool IsDownThisFrame(SDL_GameControllerButton button, int controllerId = 0) {
            if (m_ControllerStates.empty()) {
                return false;
            }
            return m_ControllerStates.at(controllerId).buttonsPressed.at(button);
        }

        bool IsUpThisFrame(SDL_GameControllerButton button, int controllerId = 0) {
            if (m_ControllerStates.empty()) {
                return false;
            }
            return m_ControllerStates.at(controllerId).buttonsReleased.at(button);
        }

        bool IsHeld(SDL_GameControllerButton button, int controllerId = 0) {
            if (m_ControllerStates.empty()) {
                return false;
            }
            return m_ControllerStates.at(controllerId).currentButtons.at(button);
        }

        bool IsDown(SDL_GameControllerButton button, int controllerId = 0) {
            return IsDownThisFrame(button, controllerId) || IsHeld(button, controllerId);
        }

    private:
        struct SDL_CONTROLLER_STATE {
            std::unordered_map<SDL_GameControllerButton, bool> previousButtons;
            std::unordered_map<SDL_GameControllerButton, bool> currentButtons;
            std::unordered_map<SDL_GameControllerButton, bool> buttonsPressed;
            std::unordered_map<SDL_GameControllerButton, bool> buttonsReleased;
        };

        void UpdateControllerInfo() {
            for (auto& [id, controller] : m_Controllers) {
                auto& state = m_ControllerStates[id];
                state.previousButtons = state.currentButtons;
                state.currentButtons.clear();
                state.buttonsPressed.clear();
                state.buttonsReleased.clear();

                for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i) {
                    SDL_GameControllerButton button = static_cast<SDL_GameControllerButton>(i);
                    bool pressed = SDL_GameControllerGetButton(controller, button);
                    state.currentButtons[button] = pressed;

                    bool wasPressed = state.previousButtons[button];
                    state.buttonsPressed[button] = pressed && !wasPressed;
                    state.buttonsReleased[button] = !pressed && wasPressed;
                }
            }
        }

        std::unordered_map<int, SDL_GameController*> m_Controllers;
        std::unordered_map<int, SDL_CONTROLLER_STATE> m_ControllerStates;
    };

    bool fovy::InputManager::IsControllerButtonDown(int button) const {
        return m_pImpl->IsDown(static_cast<SDL_GameControllerButton>(button));
    }

    bool fovy::InputManager::IsControllerButtonUp(int button) const {
        return m_pImpl->IsUpThisFrame(static_cast<SDL_GameControllerButton>(button));
    }

    bool fovy::InputManager::IsControllerButtonPressed(int button) const {
        return m_pImpl->IsDownThisFrame(static_cast<SDL_GameControllerButton>(button));
    }

    bool fovy::InputManager::IsControllerButtonReleased(int button) const {
        return m_pImpl->IsUpThisFrame(static_cast<SDL_GameControllerButton>(button));
    }

    void InputManager::RemoveAllBindings() {
        m_Bindings.clear();
    }

    void fovy::InputManager::HandleControllerInput() {
        m_pImpl->HandleController(m_Bindings);
    }

    fovy::InputManager::InputManager() {
        m_pImpl = std::make_unique<ControllerImpl>();
    }

    fovy::InputManager::~InputManager() = default;
}

#endif // SDLCONTROLLERIMPL_H
