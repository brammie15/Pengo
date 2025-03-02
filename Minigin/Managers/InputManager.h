#pragma once
#include <unordered_map>

#include "Singleton.h"

namespace dae {
    class InputManager final: public Singleton<InputManager> {
    public:
        bool ProcessInput();
        bool IsKeyDown(SDL_Scancode key);
        bool IsKeyUp(SDL_Scancode key);
        bool IsKeyPressed(SDL_Scancode key);
    private:
        std::unordered_map<SDL_Scancode, bool> m_CurrentKeyState;
        std::unordered_map<SDL_Scancode, bool> m_PreviousKeyState;
    };
}
