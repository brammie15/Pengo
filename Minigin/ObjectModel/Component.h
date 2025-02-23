#ifndef COMPONENT_H
#define COMPONENT_H

#include "Object.h"

namespace dae {
    class GameObject;
    class Transform;

    class Component: public Object {
    public:

        ~Component() override = default;

        Component(const Component& other) = delete;
        Component(Component&& other) noexcept = delete;
        Component& operator=(const Component& other) = delete;
        Component& operator=(Component&& other) noexcept = delete;

        [[nodiscard]] bool isEnabled() const { return m_IsEnabled; }
        [[nodiscard]] GameObject *GetGameObject() const { return m_ParentGameObjectPtr; }
        [[nodiscard]] Transform& GetTransform() const;

        void Destroy() override;
        virtual void SetEnabled(bool enabled);
        virtual void Update() = 0;
        virtual void LateUpdate();
        virtual void FixedUpdate();
        virtual void ImGuiInspector();


        virtual void Render();
    protected:
        explicit Component(GameObject& pParent, const std::string& name = "Component");

    private:
        GameObject* m_ParentGameObjectPtr{};
        bool m_IsEnabled{true};
    };
} // dae

#endif //COMPONENT_H
