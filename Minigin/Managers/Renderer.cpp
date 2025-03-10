#include <stdexcept>
#include <cstring>
#include "Renderer.h"

#include "implot.h"
#include "Scene/SceneManager.h"
#include "Texture2D.h"

int GetOpenGLDriverIndex() {
    auto openglIndex = -1;
    const auto driverCount = SDL_GetNumRenderDrivers();
    for (auto i = 0; i < driverCount; i++) {
        SDL_RendererInfo info;
        if (!SDL_GetRenderDriverInfo(i, &info))
            if (!strcmp(info.name, "opengl"))
                openglIndex = i;
    }
    return openglIndex;
}

void fovy::Renderer::Init(SDL_Window* window) {
    m_window = window;
    m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr) {
        throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
    ImGui_ImplOpenGL3_Init();
}

void fovy::Renderer::Render() const {
    const auto& color = GetBackgroundColor();
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_renderer);

    SceneManager::GetInstance().Render();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // hint: something should come here :)
    SceneManager::GetInstance().RenderImgui();
    // ImGui::ShowDemoWindow();

    ImGui::Render();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_RenderPresent(m_renderer);


}

void fovy::Renderer::Destroy() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    if (m_renderer != nullptr)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
}

void fovy::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const {
    SDL_Rect dst{};
    dst.x = static_cast<int>(x);
    dst.y = static_cast<int>(y);
    SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
    SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void fovy::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width,
                                  const float height) const {
    SDL_Rect dst{};
    dst.x = static_cast<int>(x);
    dst.y = static_cast<int>(y);
    dst.w = static_cast<int>(width);
    dst.h = static_cast<int>(height);
    SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

SDL_Window* fovy::Renderer::GetSDLWindow() const {
    return m_window;
}

SDL_Renderer *fovy::Renderer::GetSDLRenderer() const { return m_renderer; }
