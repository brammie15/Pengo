#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "Direction.h"
#include "Event.h"
#include "PengoStates.h"
#include "../GridComponent.h"
#include "../../../Minigin/Components/SpriteRenderer.h"
#include "ObjectModel/Component.h"


namespace pengo {


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

        void Move(fovy::Direction direction);
        void Push();

        [[nodiscard]] const fovy::Event<fovy::Direction>& GetOnPushEvent() const { return m_onPushEvent; }

        fovy::Event<fovy::Direction>& GetWallPushEvent() { return m_onWallPushEvent; }

    private:
        GridComponent* m_pGrid{ nullptr };

        glm::vec2 m_direction{ 0.0f, 0.0f };

        std::unique_ptr<pengo::PengoState> m_CurrentState{ nullptr };

        fovy::SpriteRenderer* m_spriteRenderer;

        fovy::Event<fovy::Direction> m_onPushEvent{};
        fovy::Event<fovy::Direction> m_onWallPushEvent{};
    };
}

#endif // PLAYERCOMPONENT_H 