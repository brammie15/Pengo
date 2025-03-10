#include "InputManager.h"
#include <SDL.h>

#include <iostream>
#include <SDL_syswm.h>

#include "imgui_impl_sdl2.h"
#include "Managers/Renderer.h"


bool fovy::InputManager::ProcessInput() {
    SDL_Event e;
    m_PreviousKeyState = m_CurrentKeyState;
    HandleControllerInput();
    HandleKeyboardHeld();

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return false;
        }

        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE &&
            e.window.windowID == SDL_GetWindowID(Renderer::GetInstance().GetSDLWindow())) {
            return false;
        }

        if (HandleKeyboardUpDown(e)) {
            continue;
        }

        if (ImGui_ImplSDL2_ProcessEvent(&e)) {
            continue;
        }
    }

    return true;
}

bool fovy::InputManager::IsKeyDown(SDL_Scancode key) {
    return m_CurrentKeyState[key];
}

bool fovy::InputManager::IsKeyUp(SDL_Scancode key) {
    return !m_CurrentKeyState[key];
}

bool fovy::InputManager::IsKeyPressed(SDL_Scancode key) {
    const bool curr = m_CurrentKeyState[key];
    const bool prev = m_PreviousKeyState[key];
    return curr && !prev;
}

void fovy::InputManager::HandleKeyboardHeld() {
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);

    for (auto&& bind: m_Bindings) {
        for (auto&& keyboardKey: bind.action.keyboardButtons) {
            if (bind.buttonState == ButtonState::Down and keyState[keyboardKey]) {
                bind.command->Execute();
                break;
            }
        }
    }
}

bool fovy::InputManager::HandleKeyboardUpDown(const SDL_Event& event) {
    switch (event.type) {
        case SDL_KEYDOWN:
            for (auto&& bind: m_Bindings)
                bind.ExecuteKeyboard(ButtonState::Pressed, event.key.keysym.scancode);
            m_CurrentKeyState[event.key.keysym.scancode] = true;
            return true;

        case SDL_KEYUP:
            for (auto&& bind: m_Bindings)
                bind.ExecuteKeyboard(ButtonState::Released, event.key.keysym.scancode);
            m_CurrentKeyState[event.key.keysym.scancode] = false;
            return true;
    }
    return false;
}
