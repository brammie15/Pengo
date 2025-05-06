#include "PlayerComponent.h"

#include <iostream>

#include "Timer.h"
#include "Input/InputManager.h"
#include "Managers/Renderer.h"

void fovy::PlayerComponent::Destroy() {
    Component::Destroy();
    m_pGrid->SetOccupant(m_GridPosition, nullptr);
}

fovy::PlayerComponent::PlayerComponent(GameObject& parent, GridComponent* grid)
    : Component(parent, "PlayerComponent")
    , m_pGrid(grid)
    , m_TargetPosition(m_pGrid->WorldPositionFromGrid(glm::ivec2(0,0))) {
    // Initialize player at grid position (0,0)
    GetTransform().SetLocalPosition(m_TargetPosition);
}

void fovy::PlayerComponent::Update() {
    if (m_nextDirection != MoveDirection::None && m_CurrentState == PlayerState::Idle) {
        MoveDir(MoveDirectionToVec(m_nextDirection));
        m_nextDirection = MoveDirection::None;
    }

    if (m_CurrentState != PlayerState::Idle) {
        GetTransform().SetLocalPosition(glm::mix(GetTransform().GetLocalPosition(), m_TargetPosition, m_MoveSpeed * Time::GetInstance().DeltaTime()));
        if (glm::distance(GetTransform().GetLocalPosition(), m_TargetPosition) < 0.1f) {
            GetTransform().SetLocalPosition(m_TargetPosition);
            m_CurrentState = PlayerState::Idle;
        }
    }
}

void fovy::PlayerComponent::Render() {
    const glm::vec3 pos = GetTransform().GetWorldPosition();
    const float halfSize = m_CubeSize / 2.0f;
    
    SDL_Color color = {255, 255, 255, 255}; // White color
    
    Renderer::GetInstance().RenderRect(
        pos.x - halfSize, pos.y - halfSize,
        m_CubeSize * m_pGrid->GetCellSize(), m_CubeSize * m_pGrid->GetCellSize(),
        color, true
    );
}

void fovy::PlayerComponent::ImGuiInspector() {
    if (ImGui::TreeNode("PlayerComponent")) {

        // Display the current grid position
        ImGui::Text("Current Grid Position: ");
        ImGui::SameLine();
        ImGui::Text("X: %d, Y: %d", m_GridPosition.x, m_GridPosition.y);
        ImGui::Separator();
        ImGui::Text("Current State: ");
        ImGui::SameLine();
        switch (m_CurrentState) {
            case PlayerState::Idle:
                ImGui::Text("Idle");
                break;
            case PlayerState::MovingUp:
                ImGui::Text("Moving Up");
                break;
            case PlayerState::MovingDown:
                ImGui::Text("Moving Down");
                break;
            case PlayerState::MovingLeft:
                ImGui::Text("Moving Left");
                break;
            case PlayerState::MovingRight:
                ImGui::Text("Moving Right");
                break;
        }
        ImGui::Separator();
        ImGui::Text("Target Position: ");
        ImGui::SameLine();
        ImGui::Text("X: %.2f, Y: %.2f", m_TargetPosition.x, m_TargetPosition.y);

        ImGui::TreePop();
    }
}

void fovy::PlayerComponent::Move(MoveDirection dir) {
    m_nextDirection = dir;
}

void fovy::PlayerComponent::MoveDir(glm::ivec2 dir) {
    const glm::ivec2 newGridPos = m_GridPosition + dir;
    if (!m_pGrid->IsWithinBounds(newGridPos)) {
        m_CurrentState = PlayerState::Idle;
        std::cout << "Moving out of grid" << std::endl;
        return;
    }

    if (m_pGrid->IsOccupied(newGridPos)) {
        m_CurrentState = PlayerState::Idle;
        std::cout << "Cell is occupied" << std::endl;
        return;
    }

    m_pGrid->SetOccupant(m_GridPosition, nullptr);
    m_GridPosition = newGridPos;
    m_TargetPosition = m_pGrid->WorldPositionFromGrid(m_GridPosition);
    m_pGrid->SetOccupant(m_GridPosition, GetGameObject());
    m_CurrentState = MoveDirectionToState(m_nextDirection);
}

glm::ivec2 fovy::PlayerComponent::MoveDirectionToVec(MoveDirection dir) {
    switch (dir) {
        case MoveDirection::Up: return {0, -1};
        case MoveDirection::Down: return {0, 1};
        case MoveDirection::Left: return {-1, 0};
        case MoveDirection::Right: return {1, 0};
        default: return {0, 0};
    }
}

fovy::PlayerState fovy::PlayerComponent::MoveDirectionToState(MoveDirection dir) {
    switch (dir) {
        case MoveDirection::Up: return PlayerState::MovingUp;
        case MoveDirection::Down: return PlayerState::MovingDown;
        case MoveDirection::Left: return PlayerState::MovingLeft;
        case MoveDirection::Right: return PlayerState::MovingRight;
        default: return PlayerState::Idle;
    }
}
