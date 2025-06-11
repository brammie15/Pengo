#include "./PengoComponent.h"

#include <iostream>

#include "PengoStates.h"
#include "Timer.h"
#include "../Tile/IceBlockComponent.h"
#include "Input/InputManager.h"
#include "Managers/Renderer.h"



void pengo::PengoComponent::Destroy() {
    Component::Destroy();
}

pengo::PengoComponent::PengoComponent(fovy::GameObject& parent, GridComponent* grid)
    : Component(parent, "PlayerComponent")
    , m_pGrid(grid) {

    m_spriteRenderer = this->GetGameObject()->GetComponent<fovy::SpriteRenderer>();
    if (m_spriteRenderer == nullptr) {
        std::cerr << "Spriterenderer found" << std::endl;
    }

    m_CurrentState = std::make_unique<PengoIdleState>();
    m_CurrentState->Enter(this);

    const glm::vec2 gridPos = m_pGrid->WorldPositionFromGrid({0,0});
    GetGameObject()->GetTransform().SetLocalPosition(glm::vec3(gridPos, 0.0f));
}

void pengo::PengoComponent::Update() {
    auto newState = m_CurrentState->Update(this);
    if (newState) {
        m_CurrentState->Exit(this);
        m_CurrentState = std::move(newState);
        m_CurrentState->Enter(this);
    }
}

void pengo::PengoComponent::Render() {
    // const glm::vec3 pos = GetTransform().GetWorldPosition();
    // const float halfSize = m_CubeSize / 2.0f;
    //
    // SDL_Color color = {255, 255, 255, 255}; // White color
    //
    // Renderer::GetInstance().RenderRect(
    //     pos.x - halfSize, pos.y - halfSize,
    //     m_CubeSize * m_pGrid->GetCellSize().x, m_CubeSize * m_pGrid->GetCellSize().y,
    //     color, true
    // );
}

void pengo::PengoComponent::ImGuiInspector() {
    if (ImGui::TreeNode("PlayerComponent")) {
        ImGui::Text("Current State: ");
        ImGui::SameLine();
        ImGui::Text("Current State - %s", m_CurrentState->GetName().c_str());

        ImGui::Separator();

        ImGui::Text("Current Direction: ");
        ImGui::SameLine();
        ImGui::Text("X: %.2f, Y: %.2f", m_direction.x, m_direction.y);

        ImGui::TreePop();
    }
}

void pengo::PengoComponent::Move(fovy::Direction direction) {
    if (direction != fovy::Direction::None) {
        switch (direction) {
            case fovy::Direction::Up:
                m_direction = glm::vec2(0.0f, -1.0f);
                break;
            case fovy::Direction::Down:
                m_direction = glm::vec2(0.0f, 1.0f);
                break;
            case fovy::Direction::Left:
                m_direction = glm::vec2(-1.0f, 0.0f);
                break;
            case fovy::Direction::Right:
                m_direction = glm::vec2(1.0f, 0.0f);
                break;
            default:
                break;
        }
    }
    auto newState = m_CurrentState->OnMove(this, m_direction);
    if (newState) {
        m_CurrentState->Exit(this);
        m_CurrentState = std::move(newState);
        m_CurrentState->Enter(this);
    }
}

void pengo::PengoComponent::Push() {
    auto newState = m_CurrentState->OnPush(this);
    if (newState) {
        m_CurrentState->Exit(this);
        m_CurrentState = std::move(newState);
        m_CurrentState->Enter(this);
    }
}

