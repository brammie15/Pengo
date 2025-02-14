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

        void Destroy() override;

        void CleanupComponents();

        [[nodiscard]] Transform& GetTransform() { return m_TransformPtr; };

        GameObject() = default;
        ~GameObject() override;

        GameObject(const GameObject& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject& operator=(GameObject&& other) = delete;

        template <typename Component, typename... Args>
        Component *AddComponent(Args&&... args) {
            auto& addedComponent = m_Components.emplace_back(
                std::make_unique<Component>(this, std::forward<Args>(args)...));

            return reinterpret_cast<Component*>(addedComponent.get());
        }

        template <typename Component>
        Component *GetComponent() {
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
       bool HasComponent() {
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
        Transform m_TransformPtr{};
        std::vector<std::unique_ptr<Component>> m_Components{};
    };
}
