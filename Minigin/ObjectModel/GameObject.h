#pragma once
#include <memory>
#include <vector>

#include "Component.h"
#include "Object.h"
#include "Transform.h"

namespace dae {
    class Scene;
    class Texture2D;

    class GameObject final: public Object {
    public:
        friend class SceneManager;
        friend class Scene;

        void Update();
        void LateUpdate();
        void FixedUpdate();

        void Render() const;

        void ImGuiRender();

        void Destroy() override;

        void CleanupComponents();

        [[nodiscard]] std::vector<std::unique_ptr<Component>>& GetComponents() { return m_Components; }

        [[nodiscard]] Transform& GetTransform() { return m_TransformPtr; };

        explicit GameObject(const std::string& name = "GameObject");
        ~GameObject() override;

        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) = delete;

        template <typename Component, typename... Args>
        requires std::constructible_from<Component, GameObject&, Args...>
        Component *AddComponent(Args&&... args) {
            auto& addedComponent = m_Components.emplace_back(
                std::make_unique<Component>(*this, std::forward<Args>(args)...));

            return reinterpret_cast<Component*>(addedComponent.get());
        }

        template <typename Component>
        [[nodiscard]] Component *GetComponent() {
            for (const auto& component: m_Components) {
                if (auto casted = dynamic_cast<Component*>(component.get())) {
                    return casted;
                }
            }
            return nullptr;
        }

        template <typename Component>
        Component *DestroyComponent() {
            for (const auto& component: m_Components) {
                if (auto casted = dynamic_cast<Component*>(component.get())) {
                    casted->Destroy();
                    return casted;
                }
            }
            return nullptr;
        }

       template <typename Component>
       [[nodiscard]] bool HasComponent() {
            for (const auto& component: m_Components) {
                if (auto casted = dynamic_cast<Component*>(component.get())) {
                    return true;
                }
            }
            return false;
        }

    private:
        bool m_Active{true};
        Scene* m_Scene{};
        Transform m_TransformPtr{this};
        std::vector<std::unique_ptr<Component>> m_Components{};
    };
}
