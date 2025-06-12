#include "PengoStates.h"

#include <iostream>

#include "PengoComponent.h"
#include "Timer.h"
#include "../Tile/IceBlockComponent.h"
#include "../../../Minigin/Components/SpriteRenderer.h"
#include "ObjectModel/GameObject.h"

void pengo::PengoMovingState::Enter(pengo::PengoComponent* comp) {
    const auto* gridComponent = comp->GetGrid();
    const glm::vec2 dir = comp->GetCurrentDirection();

    const auto gridPostion = gridComponent->GridPositionFromWorld(comp->GetGameObject()->GetTransform().GetWorldPosition());
    const auto nextGridPosition = gridPostion + glm::ivec2{static_cast<int>(dir.x), static_cast<int>(dir.y)};
    m_isValidMove = gridComponent->IsWithinBounds(nextGridPosition) && !gridComponent->IsOccupied(nextGridPosition);
    m_TargetPosition = gridComponent->WorldPositionFromGrid(nextGridPosition);
    m_StartPosition = comp->GetGameObject()->GetTransform().GetWorldPosition();
}

void pengo::PengoMovingState::Exit(pengo::PengoComponent*) {
}

std::unique_ptr<pengo::PengoState> pengo::PengoMovingState::Update(pengo::PengoComponent* pengo) {
    if (!m_isValidMove) {
        return std::make_unique<PengoIdleState>();
    }

    m_moveTimer += static_cast<float>(fovy::Time::GetInstance().DeltaTime());
    pengo->GetGameObject()->GetTransform().SetWorldPosition(glm::vec3(
        glm::mix(m_StartPosition, m_TargetPosition, m_moveTimer / m_moveDuration), 0));

    pengo->GetGameObject()->GetComponent<fovy::SpriteRenderer>()->PlayAnimation(std::string("walk_") +
                                                                                (pengo->GetCurrentDirection().x > 0 ? "right" : pengo->GetCurrentDirection().x < 0 ? "left" : pengo->GetCurrentDirection().y > 0 ? "down" : "up"));

    if (m_moveTimer >= m_moveDuration) {
        pengo->GetGameObject()->GetTransform().SetWorldPosition(glm::vec3(m_TargetPosition, 0.0f));

        return std::make_unique<PengoIdleState>();
    }

    if (m_moveTimer / m_moveDuration >= 0.5f && !m_updatedGrid) {
        auto* grid = pengo->GetGrid();
        const glm::ivec2 oldGridPosition = grid->GridPositionFromWorld(glm::vec3(m_StartPosition, 0.0f));
        grid->SetOccupant(oldGridPosition, nullptr);

        const auto newGridPosition = grid->GridPositionFromWorld(glm::vec3(m_TargetPosition, 0.0f));
        grid->SetOccupant(newGridPosition, pengo->GetGameObject());
        m_updatedGrid = true;
    }

    return nullptr;
}

std::unique_ptr<pengo::PengoState> pengo::PengoMovingState::OnMove(pengo::PengoComponent*, glm::ivec2) {
    return nullptr;
}

std::unique_ptr<pengo::PengoState> pengo::PengoMovingState::OnPush(pengo::PengoComponent*) {
    return nullptr;
}

void pengo::PengoIdleState::Enter(PengoComponent* comp) {
    const auto currentDirection = comp->GetCurrentDirection();
    auto* spriteRenderer = comp->GetGameObject()->GetComponent<fovy::SpriteRenderer>();

    spriteRenderer->PlayAnimation(std::string("idle_") +
                                  (comp->GetCurrentDirection().x > 0 ? "right" : comp->GetCurrentDirection().x < 0 ? "left" : comp->GetCurrentDirection().y > 0 ? "down" : "up"));
}

void pengo::PengoIdleState::Exit(PengoComponent*) {
    std::cout << "IdleState Exit" << std::endl;
}

std::unique_ptr<pengo::PengoState> pengo::PengoIdleState::Update(PengoComponent*) {
    return nullptr;
}

std::unique_ptr<pengo::PengoState> pengo::PengoIdleState::OnMove(pengo::PengoComponent*, glm::ivec2) {
    return std::make_unique<PengoMovingState>();
}

std::unique_ptr<pengo::PengoState> pengo::PengoIdleState::OnPush(pengo::PengoComponent*) {
    return std::make_unique<PengoPushState>();
}

void pengo::PengoPushState::Enter(PengoComponent* pengo) {
    auto gridPos = pengo->GetGrid()->GridPositionFromWorld(pengo->GetGameObject()->GetTransform().GetWorldPosition());
    gridPos += glm::ivec2{static_cast<int>(pengo->GetCurrentDirection().x), static_cast<int>(pengo->GetCurrentDirection().y)};
    if (!pengo->GetGrid()->IsWithinBounds(gridPos)) {
        pengo->GetWallPushEvent().Invoke(fovy::VectorToDirection(pengo->GetCurrentDirection()));
    }

    if (!pengo->GetGrid()->IsWithinBounds(gridPos) || !pengo->GetGrid()->IsOccupied(gridPos)) {

        m_validPush = false;
        std::cerr << "Invalid push: out of bounds or no ice block at target position." << std::endl;
    } else {
        auto* iceBlock = pengo->GetGrid()->GetCell(gridPos).occupant;
        if (iceBlock) {
            if (auto* iceBlockComponent = iceBlock->GetComponent<pengo::IceBlockComponent>()) {
                iceBlockComponent->Push(glm::ivec2{static_cast<int>(pengo->GetCurrentDirection().x), static_cast<int>(pengo->GetCurrentDirection().y)});
                m_validPush = true;
            }
        }
    }
}

void pengo::PengoPushState::Exit(PengoComponent*) {
}

std::unique_ptr<pengo::PengoState> pengo::PengoPushState::Update(pengo::PengoComponent*) {
    return std::make_unique<PengoIdleState>();
}

std::unique_ptr<pengo::PengoState> pengo::PengoPushState::OnMove(pengo::PengoComponent*, glm::ivec2) {
    return nullptr;
}

std::unique_ptr<pengo::PengoState> pengo::PengoPushState::OnPush(pengo::PengoComponent*) {
    return nullptr;
}
