#pragma once
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "../Singleton.h"

namespace fovy {
    class Scene;

    class SceneManager final : public Singleton<SceneManager> {
    public:
        Scene &CreateScene(const std::string &name);

        //TODO: Verry bad fix
        Scene& GetCurrentScene() const { return *m_scenes[m_ActiveSceneIndex]; }

        void Update();
        void FixedUpdate();
        void LateUpdate();

        void Render();
        void RenderImgui();

        void HandleGameObjectDestroy();
        void DestroyGameObjects();
        void UnloadAllScenes();
        void HandleSceneDestroy();

        void HandleScene();

        void Destroy();

        void SwitchScene(int index);

    private:
        friend class Singleton<SceneManager>;

        SceneManager() = default;

        int m_ActiveSceneIndex{0};

        std::vector<std::shared_ptr<Scene> > m_scenes;
    };
}
