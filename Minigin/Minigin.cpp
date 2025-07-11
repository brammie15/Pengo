#include <stdexcept>
#include <sstream>
#include <iostream>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"

#include <thread>

#include "Input/InputManager.h"
#include "Scene/SceneManager.h"
#include "Managers/Renderer.h"
#include "Managers/ResourceManager.h"
#include "Timer.h"

SDL_Window* g_window{};

void LogSDLVersion(const std::string& message, const SDL_version& v) {
#if WIN32
    std::stringstream ss;
    ss << message << (int)v.major << "." << (int)v.minor << "." << (int)v.patch << "\n";
    OutputDebugString(ss.str().c_str());
#else
	std::cout << message << (int)v.major << "." << (int)v.minor << "." << (int)v.patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__
#include "emscripten.h"

void LoopCallback(void* arg)
{
	static_cast<dae::Minigin*>(arg)->RunOneFrame();
}
#endif

// Why bother with this? Because sometimes students have a different SDL version installed on their pc.
// That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
// These entries in the debug output help to identify that issue.
void PrintSDLVersion() {
    SDL_version version{};
    SDL_VERSION(&version);
    LogSDLVersion("We compiled against SDL version ", version);

    SDL_GetVersion(&version);
    LogSDLVersion("We linked against SDL version ", version);

    SDL_IMAGE_VERSION(&version);
    LogSDLVersion("We compiled against SDL_image version ", version);

    version = *IMG_Linked_Version();
    LogSDLVersion("We linked against SDL_image version ", version);

    SDL_TTF_VERSION(&version)
    LogSDLVersion("We compiled against SDL_ttf version ", version);

    version = *TTF_Linked_Version();
    LogSDLVersion("We linked against SDL_ttf version ", version);
}

fovy::Minigin::Minigin(const std::filesystem::path& dataPath) {
    PrintSDLVersion();

    //Thanks Stackoverflow
    if (AllocConsole() == TRUE) {
        FILE* empty{};
        freopen_s(&empty, "CONOUT$", "w", stdout);
        freopen_s(&empty, "CONOUT$", "w", stderr);
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
    }

    // g_window = SDL_CreateWindow(
    //     "Programming 4 assignment",
    //     SDL_WINDOWPOS_CENTERED,
    //     SDL_WINDOWPOS_CENTERED,
    //     640,
    //     480,
    //     SDL_WINDOW_OPENGL
    // );
    g_window = SDL_CreateWindow(
        "Programming 4 assignment",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        224 * 3,
        288  * 3,
        SDL_WINDOW_OPENGL
    );
    if (g_window == nullptr) {
        throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
    }

    Renderer::GetInstance().Init(g_window);
    ResourceManager::GetInstance().Init(dataPath);
}

fovy::Minigin::~Minigin() {
    SceneManager::GetInstance().Destroy();
    Renderer::GetInstance().Destroy();
    SDL_DestroyWindow(g_window);
    g_window = nullptr;
    SDL_Quit();
}

void fovy::Minigin::Run(const std::function<void()>& load) {
    load();
    Time::GetInstance().Update();
#ifndef __EMSCRIPTEN__
    while (!m_quit)
        RunOneFrame();
#else
	emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
}

void fovy::Minigin::RunOneFrame() {
    auto& Time{Time::GetInstance()};
    auto& SceneManager{ SceneManager::GetInstance() };
    Time.Update();

    const bool shouldContinue = InputManager::GetInstance().ProcessInput();
    if (!shouldContinue) {
        m_quit = true;
        return;
    }

    m_AccumulatedTime += Time.DeltaTime();

    while (m_AccumulatedTime >= Time.FixedDeltaTime()) {
        m_AccumulatedTime -= Time.FixedDeltaTime();
        SceneManager.FixedUpdate();
    }

    SceneManager.Update();
    SceneManager.LateUpdate();

    Renderer::GetInstance().Render();

    SceneManager.HandleScene();

    std::this_thread::sleep_for(Time.SleepDuration());
}
