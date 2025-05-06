#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include <SDL.h>

#include "ObjectModel/Component.h"
#include "GridComponent.h"

namespace fovy {
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

    class PlayerComponent final : public Component {
    public:
        void Destroy() override;

        explicit PlayerComponent(GameObject& parent, GridComponent* grid);
        
        void Update() override;
        void Render() override;
        void ImGuiInspector() override;

        void Move(MoveDirection dir);

    private:
        GridComponent* m_pGrid{ nullptr };
        glm::ivec2 m_GridPosition{ 0, 0 };
        glm::vec3 m_TargetPosition;
        const float m_CubeSize{ 1.f };
        const float m_MoveSpeed{ 20.0f };

        PlayerState m_CurrentState{ PlayerState::Idle };
        MoveDirection m_nextDirection{ MoveDirection::None };

        void MoveDir(glm::ivec2 dir);
        static glm::ivec2 MoveDirectionToVec(MoveDirection dir);
        static PlayerState MoveDirectionToState(MoveDirection dir);
    };
}

#endif // PLAYERCOMPONENT_H 