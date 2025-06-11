#include "SnoBeeComponent.h"

#include <iostream>

#include <imgui.h>
#include "ObjectModel/GameObject.h"

void pengo::SnoBeeComponent::Destroy() {
    const glm::ivec2 currentGridPosition = m_pGrid->GridPositionFromWorld(GetGameObject()->GetTransform().GetLocalPosition());
    m_pGrid->SetOccupant(currentGridPosition, nullptr);
    Component::Destroy();
}

pengo::SnoBeeComponent::SnoBeeComponent(fovy::GameObject& parent, GridComponent* grid, glm::vec2 startPosition): Component(parent, "SnoBeeComponent"), m_pGrid{grid} {
    m_spriteRenderer = this->GetGameObject()->GetComponent<fovy::SpriteRenderer>();
    if (m_spriteRenderer == nullptr) {
        std::cerr << "SpriteRenderer not found" << std::endl;
    }

    const auto worldGridPos = m_pGrid->WorldPositionFromGrid(startPosition);
    GetGameObject()->GetTransform().SetLocalPosition(worldGridPos);
    m_pGrid->SetOccupant(m_pGrid->GridPositionFromWorld(worldGridPos), this->GetGameObject());

    m_CurrentState = std::make_unique<pengo::SnoBeeSpawnState>();
}

void pengo::SnoBeeComponent::Update() {
    auto newState = m_CurrentState->Update(this);
    if (newState) {
        m_CurrentState->Exit(this);
        m_CurrentState = std::move(newState);
        m_CurrentState->Enter(this);
    }
}

void pengo::SnoBeeComponent::Render() {
}

void pengo::SnoBeeComponent::ImGuiInspector() {
    if (ImGui::TreeNode("SnoBeeComponent")) {
        ImGui::Text("Current State: ");
        ImGui::SameLine();
        ImGui::Text("Current State - %s", m_CurrentState ? m_CurrentState->GetName().c_str() : "None");
        ImGui::TreePop();
    }
}

void pengo::SnoBeeComponent::Move(fovy::Direction direction) {
    if (direction == fovy::Direction::None) {
        return;
    }

    switch (direction) {
        case fovy::Direction::Up:
            m_direction = {0.0f, -1.0f};
            break;
        case fovy::Direction::Down:
            m_direction = {0.0f, 1.0f};
            break;
        case fovy::Direction::Left:
            m_direction = {-1.0f, 0.0f};
            break;
        case fovy::Direction::Right:
            m_direction = {1.0f, 0.0f};
            break;
        default:
            return; // Invalid direction
    }

    auto newState = m_CurrentState->OnMove(this, glm::ivec2{static_cast<int>(direction)});
    if (newState) {
        m_CurrentState->Exit(this);
        m_CurrentState = std::move(newState);
        m_CurrentState->Enter(this);
    }
}

void pengo::SnoBeeComponent::Push() {
    auto newState = m_CurrentState->OnPush(this);
    if (newState) {
        m_CurrentState->Exit(this);
        m_CurrentState = std::move(newState);
        m_CurrentState->Enter(this);
    }
}


