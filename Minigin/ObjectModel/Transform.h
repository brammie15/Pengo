#pragma once
#include <vector>
#include <glm.hpp>
#include <gtc/quaternion.hpp>

namespace fovy {
    class GameObject;
}

namespace fovy {
    class Transform final {
    public:

        explicit Transform(GameObject* owner);
        ~Transform();
        Transform(const Transform&) = delete;
        Transform(Transform&&) noexcept = delete;
        Transform& operator=(const Transform&) = delete;
        Transform& operator=(Transform&&) noexcept = delete;

        [[nodiscard]] const glm::vec3& GetWorldPosition();
        [[nodiscard]] const glm::quat& GetWorldRotation();
        [[nodiscard]] const glm::vec3& GetWorldScale();
        [[nodiscard]] const glm::mat4& GetWorldMatrix();


        void Move(const glm::vec3& move);
        void Move(double x, double y, double z);


        [[nodiscard]] const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
        [[nodiscard]] const glm::quat& GetLocalRotation() const { return m_LocalRotation; }
        [[nodiscard]] const glm::vec3& GetLocalScale() const { return m_LocalScale; }


        void SetLocalPosition(const glm::vec3& position);
        void SetLocalPosition(float x, float y, float z);

        void SetLocalRotation(float x, float y, float z);
        void SetLocalRotation(const glm::vec3& rotation);
        void SetLocalRotation(const glm::quat& rotation);

        void SetLocalScale(float x, float y, float z);
        void SetLocalScale(const glm::vec3& scale);

        void SetWorldPosition(const glm::vec3& position);
        void SetWorldPosition(float x, float y, float z);
        void SetWorldRotation(const glm::quat& rotation);
        void SetWorldRotation(const glm::vec3& rotation);
        void SetWorldRotation(float x, float y, float z);
        void SetWorldScale(double x, double y, double z);
        void SetWorldScale(const glm::vec3& scale);

        [[nodiscard]] Transform* GetParent() const { return m_Parent; }


        void SetParent(Transform* parent, bool useWorldPosition = true);

        [[nodiscard]] bool IsChild(Transform* child) const;

        [[nodiscard]] int GetChildCount() const { return static_cast<int>(m_Children.size()); }
        [[nodiscard]] const std::vector<Transform*>& GetChildren() const;
        [[nodiscard]] fovy::GameObject* GetOwner() const;

    private:
        void AddChild(Transform* transform);
        void RemoveChild(Transform* transform);


        void UpdateWorldPosition();
        void UpdateWorldRotation();
        void UpdateWorldScale();
        void UpdateWorldMatrix();

        void SetPositionDirty();
        void SetRotationDirty();
        void SetScaleDirty();

        glm::vec3 m_LocalPosition{};
        glm::quat m_LocalRotation{ glm::mat4{ 1.0f }};
        glm::vec3 m_LocalScale{ 1, 1, 1 };

        bool m_PositionDirty{true};
        bool m_RotationDirty{true};
        bool m_ScaleDirty{true};
        bool m_MatrixDirty{true};

        glm::vec3 m_WorldPosition{};
        glm::quat m_WorldRotation{ glm::mat4{ 1.0f }};
        glm::vec3 m_WorldScale{ 1, 1, 1 };

        glm::mat4 m_WorldMatrix{};


        Transform* m_Parent{};
        std::vector<Transform*> m_Children{};

        GameObject* m_Owner{};
    };
}
