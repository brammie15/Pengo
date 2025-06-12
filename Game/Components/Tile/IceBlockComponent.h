#ifndef ICEBLOCKCOMPONENT_H
#define ICEBLOCKCOMPONENT_H

#include <memory>

#include "IceblockStates.h"
#include "../GridComponent.h"
#include "ObjectModel/Component.h"

namespace pengo {
    class IceBlockState;

    class IceBlockComponent final: public fovy::Component {
    public:
        explicit IceBlockComponent(fovy::GameObject& parent, GridComponent* grid, bool isDiamond = false);
        ~IceBlockComponent() override = default;
        void Destroy() override;

        void Update() override;
        void Render() override;
        void ImGuiInspector() override;

        void Push(glm::ivec2 direction);

        glm::ivec2 GetSlideDirection() const { return m_SlideDirection; }
        GridComponent* GetGrid() const { return m_pGrid; }

        bool IsDiamond() const { return m_isDiamond; }

    private:
        GridComponent* m_pGrid;
        glm::ivec2 m_GridPosition;
        glm::vec2 m_TargetPosition;
        glm::ivec2 m_SlideDirection;

        std::unique_ptr<IceBlockState> m_CurrentState{ nullptr };

        bool m_isDiamond{ false };
    };

} // namespace fovy

#endif //ICEBLOCKCOMPONENT_H
