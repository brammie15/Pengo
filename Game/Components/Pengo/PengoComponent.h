#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include <SDL.h>

#include "PengoStates.h"
#include "ObjectModel/Component.h"
#include "../GridComponent.h"
#include "../SpriteRenderer.h"


namespace pengo {
    enum class PlayerState {
        Idle,
        MovingUp,
        MovingDown,
        MovingLeft,
        MovingRight
    };

    enum class MoveDirection{
        None,
        Up,
        Down,
        Left,
        Right
    };

    class PengoComponent final : public fovy::Component {
    public:
        void Destroy() override;

        [[nodiscard]] GridComponent* GetGrid() const { return m_pGrid; }
        explicit PengoComponent(fovy::GameObject& parent, GridComponent* grid);
        
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

        std::unique_ptr<pengo::PengoState> m_CurrentState{ nullptr };

        fovy::SpriteRenderer* m_spriteRenderer;
    };
}

#endif // PLAYERCOMPONENT_H 