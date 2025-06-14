#include "SceneManager.h"
#include "Scene.h"
#include "Input/InputManager.h"

void fovy::SceneManager::Update() {
    m_scenes[m_ActiveSceneIndex]->Update();
}

void fovy::SceneManager::FixedUpdate() {
    m_scenes[m_ActiveSceneIndex]->FixedUpdate();
}

void fovy::SceneManager::LateUpdate() {
    m_scenes[m_ActiveSceneIndex]->LateUpdate();
}

void fovy::SceneManager::Render() {
    m_scenes[m_ActiveSceneIndex]->Render();
}

void fovy::SceneManager::RenderImgui() {
    m_scenes[m_ActiveSceneIndex]->RenderImgui();
}

void fovy::SceneManager::HandleGameObjectDestroy() {
    for (auto& scene : m_scenes) {
        scene->CleanupDestroyedGameObjects();
    }
}

void fovy::SceneManager::DestroyGameObjects() {
    for (auto& scene: m_scenes) {
        scene->DestroyGameObjects();
    }
}

void fovy::SceneManager::UnloadAllScenes() {
    for (auto& scene : m_scenes) {
        scene->Unload();
    }
}

void fovy::SceneManager::HandleSceneDestroy() {
    for (auto it = m_scenes.begin(); it != m_scenes.end();) {
        if ((*it)->IsBeingUnloaded()) {
            it = m_scenes.erase(it);
        } else {
            ++it;
        }
    }
}

void fovy::SceneManager::HandleScene() {
    DestroyGameObjects();
    HandleGameObjectDestroy();
    HandleSceneDestroy();
}

void fovy::SceneManager::Destroy() {
    UnloadAllScenes();
    DestroyGameObjects();
    HandleGameObjectDestroy();
    HandleSceneDestroy();
}

void fovy::SceneManager::SwitchScene(int index) {
    InputManager::GetInstance().RemoveAllBindings();

    if (index < 0 || index >= static_cast<int>(m_scenes.size())) {
        throw std::out_of_range("Scene index out of range");
    }
    // m_scenes[m_ActiveSceneIndex]->Unload();
    m_scenes[m_ActiveSceneIndex]->UnloadBindings();
    m_ActiveSceneIndex = index;
    m_scenes[m_ActiveSceneIndex]->LoadBindings();
}

fovy::Scene &fovy::SceneManager::CreateScene(const std::string &name) {
    const auto &scene = std::shared_ptr<Scene>(new Scene(name));
    m_scenes.push_back(scene);
    return *scene;
}
