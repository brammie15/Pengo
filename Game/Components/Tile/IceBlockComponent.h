#ifndef ICEBLOCKCOMPONENT_H
#define ICEBLOCKCOMPONENT_H

#include <memory>

#include "ObjectModel/Component.h"
#include "../GridComponent.h"

namespace pengo {
    class IceBlockState;

    class IceBlockComponent : public fovy::Component {
    public:
        IceBlockComponent(fovy::GameObject& parent, GridComponent* grid);
        ~IceBlockComponent() override = default;
        void Destroy() override;

        void Update() override;
        void Render() override;
        void ImGuiInspector() override;

        void Push(glm::ivec2 direction);

        glm::ivec2 GetSlideDirection() const { return m_SlideDirection; }
        GridComponent* GetGrid() const { return m_pGrid; }

    private:
        GridComponent* m_pGrid;
        glm::ivec2 m_GridPosition;
        glm::vec2 m_TargetPosition;
        glm::ivec2 m_SlideDirection;

        std::unique_ptr<IceBlockState> m_CurrentState{ nullptr };
    };

} // namespace fovy

#endif //ICEBLOCKCOMPONENT_H
