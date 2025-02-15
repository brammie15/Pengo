#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update() {
    for (auto &scene: m_scenes) {
        scene->Update();
    }
}

void dae::SceneManager::FixedUpdate() {
    for (auto &scene: m_scenes) {
        scene->FixedUpdate();
    }
}

void dae::SceneManager::LateUpdate() {
    for (auto &scene: m_scenes) {
        scene->LateUpdate();
    }
}

void dae::SceneManager::Render() {
    for (const auto &scene: m_scenes) {
        scene->Render();
    }
}

void dae::SceneManager::RenderImgui() {
    for (const auto & scene : m_scenes) {
        scene->RenderImgui();
    }
}

void dae::SceneManager::HandleGameObjectDestroy() {
    for (auto& scene : m_scenes) {
        scene->CleanupDestroyedGameObjects();
    }
}

void dae::SceneManager::DestroyGameObjects() {
    for (auto& scene: m_scenes) {
        scene->DestroyGameObjects();
    }
}

void dae::SceneManager::UnloadAllScenes() {
    for (auto& scene : m_scenes) {
        scene->Unload();
    }
}

void dae::SceneManager::HandleSceneDestroy() {
    for (auto it = m_scenes.begin(); it != m_scenes.end();) {
        if ((*it)->IsBeingUnloaded()) {
            it = m_scenes.erase(it);
        } else {
            ++it;
        }
    }
}

void dae::SceneManager::HandleScene() {
    DestroyGameObjects();
    HandleGameObjectDestroy();
    HandleSceneDestroy();
}

void dae::SceneManager::Destroy() {
    UnloadAllScenes();
    DestroyGameObjects();
    HandleGameObjectDestroy();
    HandleSceneDestroy();
}

dae::Scene &dae::SceneManager::CreateScene(const std::string &name) {
    const auto &scene = std::shared_ptr<Scene>(new Scene(name));
    m_scenes.push_back(scene);
    return *scene;
}
