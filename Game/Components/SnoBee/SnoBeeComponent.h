#ifndef SNOBEE_H
#define SNOBEE_H

#include <memory>

#include "SnoBeeStates.h"
#include "Components/GridComponent.h"
#include "Components/SpriteRenderer.h"
#include "ObjectModel/Component.h"


namespace pengo {
    class SnoBeeComponent final : public fovy::Component {
    public:
        void Destroy() override;

        [[nodiscard]] GridComponent* GetGrid() const { return m_pGrid; }
        explicit SnoBeeComponent(fovy::GameObject& parent, GridComponent* grid, glm::vec2 startPosition);

        void Update() override;
        void Render() override;
        void ImGuiInspector() override;


        glm::vec2 GetCurrentDirection() const { return m_direction; }
        void SetCurrentDirection(const glm::vec2& direction) { m_direction = direction; }

        void Move(MoveDirection direction);
        void Push();
    private:
        GridComponent* m_pGrid{ nullptr };

        glm::vec2 m_direction{ 0.0f, 0.0f };

        std::unique_ptr<pengo::SnoBeeState> m_CurrentState{ nullptr };

        fovy::SpriteRenderer* m_spriteRenderer;
    };
}



#endif //SNOBEE_H
