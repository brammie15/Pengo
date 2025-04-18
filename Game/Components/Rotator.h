#ifndef ROTATOR_H
#define ROTATOR_H
#include <glm.hpp>

#include "ObjectModel/Component.h"
#include "ObjectModel/Transform.h"

namespace fovy {
    class Rotator final : public Component {
    public:
        explicit Rotator(GameObject& parent, float distance, float speed);

        Rotator(const Rotator& other) = delete;
        Rotator(Rotator&& other) noexcept = delete;
        Rotator& operator=(const Rotator& other) = delete;
        Rotator& operator=(Rotator&& other) noexcept = delete;

        void Update() override;

        void SetDistance(float distance) { m_Distance = distance; }
        void SetSpeed(float speed) { m_Speed = speed; }
        void SetRotatePosition(const glm::vec3& position) { GetTransform().SetLocalPosition(position); }
        void ImGuiInspector() override;

    private:
        float m_Distance{};
        float m_Speed{};
        float m_CurrentAngle{};
        glm::vec3 m_OriginalPosition{};
    };
}

#endif //ROTATOR_H
