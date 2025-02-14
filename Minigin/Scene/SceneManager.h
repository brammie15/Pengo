#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Singleton.h"

namespace dae {
    class Scene;

    class SceneManager final : public Singleton<SceneManager> {
    public:
        Scene &CreateScene(const std::string &name);

        void Update();
        void FixedUpdate();
        void LateUpdate();

        void Render();

        void HandleGameObjectDestroy();
        void DestroyGameObjects();
        void UnloadAllScenes();
        void HandleSceneDestroy();

        void HandleScene();

        void Destroy();

    private:
        friend class Singleton<SceneManager>;

        SceneManager() = default;

        std::vector<std::shared_ptr<Scene> > m_scenes;
    };
}
