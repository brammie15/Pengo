#include <SDL.h>
#include "InputManager.h"

#include "imgui_impl_sdl2.h"
#include "Renderer.h"

bool dae::InputManager::ProcessInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        ImGui_ImplSDL2_ProcessEvent(&e);

        if (e.type == SDL_QUIT) {
            return false;
        }

        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE and e.window.windowID == SDL_GetWindowID(Renderer::GetInstance().GetSDLWindow())) {
            return false;
        }
        if (e.type == SDL_KEYDOWN) {
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
        }


        // etc...
    }

    return true;
}
