#pragma once

#include <algorithm>
#include <memory>
#include <unordered_map>

#include "../Singleton.h"
#include <SDL.h>

#include "InputBinding.h"

namespace fovy {
    class InputManager final: public Singleton<InputManager> {
    public:
        InputManager();
        ~InputManager() override;

        InputManager(const InputManager& other) = delete;
        InputManager(InputManager&& other) noexcept = delete;
        InputManager& operator=(const InputManager& other) = delete;
        InputManager& operator=(InputManager&& other) noexcept = delete;

        bool ProcessInput();
        bool IsKeyDown(SDL_Scancode key);
        bool IsKeyUp(SDL_Scancode key);
        bool IsKeyPressed(SDL_Scancode key);

        bool IsControllerButtonDown(int button) const;
        bool IsControllerButtonUp(int button) const;
        bool IsControllerButtonPressed(int button) const;
        bool IsControllerButtonReleased(int button) const;


        template <typename CommandT, typename... Args>
        void AddCommand(InputAction action, ButtonState state, Args&&... args) {
            auto* command = new CommandT(std::forward<Args>(args)...);
            m_Bindings.emplace_back(state, 0, action, command);
        }

        template <typename CommandT, typename... Args>
        void AddCommand(InputAction action, ButtonState state, int controllerId, Args&&... args) {
            auto* command = new CommandT(std::forward<Args>(args)...);
            m_Bindings.emplace_back(state, controllerId, action, command);
        }

        //Remove all bindings for a specific action
        void RemoveBindings(const InputAction& action) {
            std::erase_if(m_Bindings,
                          [&action](const InputBinding& binding) { return binding.action == action; });
        }

    private:
        void HandleControllerInput();
        void HandleKeyboardHeld();

        bool HandleKeyboardUpDown(const SDL_Event& event);

        std::unordered_map<SDL_Scancode, bool> m_CurrentKeyState;
        std::unordered_map<SDL_Scancode, bool> m_PreviousKeyState;

        class ControllerImpl;
        std::unique_ptr<ControllerImpl> m_pImpl;

        std::vector<InputBinding> m_Bindings;
    };
}
