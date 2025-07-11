#ifndef SNOBEE_H
#define SNOBEE_H

#include <memory>

#include "Direction.h"
#include "SnoBeeStates.h"
#include "Components/GridComponent.h"
#include "../../../Minigin/Components/SpriteRenderer.h"
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


        [[nodiscard]] glm::vec2 GetCurrentDirection() const { return m_direction; }
        void SetCurrentDirection(const glm::vec2& direction) { m_direction = direction; }

        void Move(fovy::Direction direction);
        void Push();
        void BreakIce(fovy::Direction direction);

        [[nodiscard]] bool IsCaught() const { return m_caught; }
        void SetCaught(bool caught) { m_caught = caught; }

        [[nodiscard]] fovy::SpriteRenderer* GetSpriteRenderer() const { return m_spriteRenderer; }

        void Stun();

    private:
        GridComponent* m_pGrid{ nullptr };

        glm::vec2 m_direction{ 0.0f, 0.0f };

        std::unique_ptr<pengo::SnoBeeState> m_CurrentState{ nullptr };

        fovy::SpriteRenderer* m_spriteRenderer;

        bool m_caught{ false };
    };
}



#endif //SNOBEE_H
