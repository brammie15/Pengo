#include "IceBlockComponent.h"

#include "IceblockStates.h"
#include "Timer.h"
#include "Managers/Renderer.h"
#include "ObjectModel/GameObject.h"
#include "ObjectModel/Transform.h"

pengo::IceBlockComponent::IceBlockComponent(fovy::GameObject& parent, GridComponent* grid, bool isDiamond, bool containEgg)
    : Component(parent, "IceBlockComponent")
      , m_pGrid(grid)
      , m_GridPosition(0, 0)
      , m_TargetPosition(grid->WorldPositionFromGrid(m_GridPosition)), m_SlideDirection{}, m_isDiamond{isDiamond}, m_containsEgg{containEgg} {
    // Initialize grid position based on current world position
    m_GridPosition = m_pGrid->GridPositionFromWorld(parent.GetTransform().GetWorldPosition());
    m_TargetPosition = m_pGrid->WorldPositionFromGrid(m_GridPosition);

    // Register with grid
    if (m_pGrid->IsWithinBounds(m_GridPosition)) {
        m_pGrid->SetOccupant(m_GridPosition, &parent);
    }

    m_CurrentState = std::make_unique<pengo::IceBlockSpawnState>();
}

pengo::IceBlockComponent::~IceBlockComponent() {

}

void pengo::IceBlockComponent::Destroy() {
    auto currentGridPosition = m_pGrid->GridPositionFromWorld(GetGameObject()->GetTransform().GetWorldPosition());
    m_pGrid->SetOccupant(currentGridPosition, nullptr);
    Component::Destroy();
}

void pengo::IceBlockComponent::Start() {
    m_CurrentState->Enter(this);
}

void pengo::IceBlockComponent::Update() {
    auto newState = m_CurrentState->Update(this);
    if (newState) {
        m_CurrentState->Exit(this);
        m_CurrentState = std::move(newState);
        m_CurrentState->Enter(this);
    }
}

void pengo::IceBlockComponent::Render() {
}

void pengo::IceBlockComponent::ImGuiInspector() {
    if (ImGui::TreeNode("IceBlockComponent")) {
        ImGui::Text("Grid Position: (%d, %d)", m_GridPosition.x, m_GridPosition.y);
        ImGui::TreePop();
    }
}

void pengo::IceBlockComponent::Push(glm::ivec2 dir) {
    m_SlideDirection = dir;
    auto newState = m_CurrentState->OnPush(this);
    if (newState) {
        m_CurrentState->Exit(this);
        m_CurrentState = std::move(newState);
        m_CurrentState->Enter(this);
    }
}

void pengo::IceBlockComponent::Break() {
    auto newState = m_CurrentState->OnBreak(this);
    if (newState) {
        m_CurrentState->Exit(this);
        m_CurrentState = std::move(newState);
        m_CurrentState->Enter(this);
    }
}
