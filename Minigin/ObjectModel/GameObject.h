#pragma once
#include <memory>
#include <vector>

#include "Component.h"
#include "Object.h"
#include "Transform.h"

namespace fovy {
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

        void SetActive(bool active) {
            if (active == m_Active) {
                return;
            }

            m_Active = active;
            SetActiveDirty();
        }

        [[nodiscard]] std::vector<std::unique_ptr<Component>>& GetComponents() { return m_Components; }

        [[nodiscard]] Transform& GetTransform() { return m_TransformPtr; }

        [[nodiscard]] bool IsActiveInHierarchy();

        [[nodiscard]] bool IsActive() const { return m_Active; }

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

        template <typename Component>
        Component *GetComponentInChildren() {
            return GetComponentInChildrenRecursive<Component>(&GetTransform());
        }

    private:
        template <typename Component>
        static Component *GetComponentInChildrenRecursive(Transform* transform) {
            if (!transform) return nullptr;
            GameObject* owner = transform->GetOwner();
            if (owner) {
                if (Component* comp = owner->GetComponent<Component>()) {
                    return comp;
                }
            }
            for (Transform* child: transform->GetChildren()) {
                if (Component* found = GetComponentInChildrenRecursive<Component>(child)) {
                    return found;
                }
            }
            return nullptr;
        }

        void SetActiveDirty();

        bool m_Active{true};
        Scene* m_Scene{};
        Transform m_TransformPtr{this};
        std::vector<std::unique_ptr<Component>> m_Components{};

        bool m_ActiveDirty{true};
        bool m_ActiveInHierarchy{true}; //Derived
        void UpdateActiveState();
    };
}
