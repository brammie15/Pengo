#include <SDL.h>
#include "InputManager.h"

#include "imgui_impl_sdl2.h"
#include "Renderer.h"

bool dae::InputManager::ProcessInput() {
    SDL_Event e;
    m_PreviousKeyState = m_CurrentKeyState;

    while (SDL_PollEvent(&e)) {
        ImGui_ImplSDL2_ProcessEvent(&e);

        if (e.type == SDL_QUIT) {
            return false;
        }

        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE and e.window.windowID ==
            SDL_GetWindowID(Renderer::GetInstance().GetSDLWindow())) {
            return false;
        }
        if (e.type == SDL_KEYDOWN) {
            m_CurrentKeyState[e.key.keysym.scancode] = true;
        }
        if (e.type == SDL_KEYUP) {
            m_CurrentKeyState[e.key.keysym.scancode] = false;
        }
        // etc...
    }

    return true;
}

bool dae::InputManager::IsKeyDown(SDL_Scancode key) {
    return m_CurrentKeyState[key];
}

bool dae::InputManager::IsKeyUp(SDL_Scancode key) {
    return !m_CurrentKeyState[key];
}

bool dae::InputManager::IsKeyPressed(SDL_Scancode key) {
    const bool curr = m_CurrentKeyState[key];
    const bool prev = m_PreviousKeyState[key];
    return curr && !prev;
}
