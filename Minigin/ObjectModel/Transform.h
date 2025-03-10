#pragma once
#include <glm.hpp>
#include <vector>


namespace dae {
    class GameObject;
}

namespace dae {
    class Transform final {
    public:

        explicit Transform(GameObject* owner);
        ~Transform();
        Transform(const Transform&) = delete;
        Transform(Transform&&) noexcept = delete;
        Transform& operator=(const Transform&) = delete;
        Transform& operator=(Transform&&) noexcept = delete;

        [[nodiscard]] const glm::vec3& GetWorldPosition();
        void SetWorldPosition(const glm::vec3& position);

        void Move(const glm::vec3& move);
        void Move(double x, double y, double z);


        [[nodiscard]] const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
        void SetLocalPosition(const glm::vec3& position);

        [[nodiscard]] Transform* GetParent() const { return m_Parent; }


        void SetParent(Transform* parent, bool useWorldPosition = true);

        [[nodiscard]] bool IsChild(Transform* child) const;

        [[nodiscard]] int GetChildCount() const { return static_cast<int>(m_Children.size()); }
        [[nodiscard]] const std::vector<Transform*>& GetChildren() const { return m_Children; }
        [[nodiscard]] dae::GameObject* GetOwner() const;

    private:
        void SetPositionDirty();
        void UpdateWorldPosition();
        void AddChild(Transform* transform);
        void RemoveChild(Transform* transform);

        glm::vec3 m_WorldPosition{};
        glm::vec3 m_LocalPosition{};
        bool m_PositionDirty{true};

        Transform* m_Parent{};
        std::vector<Transform*> m_Children{};

        GameObject* m_Owner{};
    };
}
