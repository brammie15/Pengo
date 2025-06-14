#include "SnoBeeStates.h"

#include <iostream>

#include "GameController.h"
#include "Timer.h"
#include "Components/Pengo/PengoComponent.h"
#include "Components/SnoBee/SnoBeeComponent.h"
#include "Components/Tile/IceBlockComponent.h"
#include "ObjectModel/GameObject.h"

void pengo::SnoBeeSpawnState::Enter(pengo::SnoBeeComponent* snoBee) {
    const auto spriteRenderer = snoBee->GetGameObject()->GetComponent<fovy::SpriteRenderer>();
    if (spriteRenderer) {
        spriteRenderer->PlayAnimation("spawn");
        m_spawnDuration = spriteRenderer->GetCurrentAnimationDuration();
    } else {
        std::cerr << "SpriteRenderer not found in SnoBeeComponent" << std::endl;
    }
}

void pengo::SnoBeeSpawnState::Exit(pengo::SnoBeeComponent* snoBee) {
    snoBee->GetGameObject()->GetComponent<fovy::SpriteRenderer>()->PlayAnimation("idle");
}

std::unique_ptr<pengo::SnoBeeState> pengo::SnoBeeSpawnState::Update(pengo::SnoBeeComponent*) {
    m_spawnTimer += static_cast<float>(fovy::Time::GetInstance().DeltaTime());
    if (m_spawnTimer >= m_spawnDuration) {
        return std::make_unique<pengo::SnoBeeIdleState>();
    }

    return nullptr;
}

std::unique_ptr<pengo::SnoBeeState> pengo::SnoBeeSpawnState::OnMove(pengo::SnoBeeComponent*, glm::ivec2) {
    return nullptr;
}

std::unique_ptr<pengo::SnoBeeState> pengo::SnoBeeSpawnState::OnPush(pengo::SnoBeeComponent*) {
    return nullptr;
}

void pengo::SnoBeeIdleState::Enter(pengo::SnoBeeComponent* snoBee) {
    const auto currentDirection = snoBee->GetCurrentDirection();
    auto* spriteRenderer = snoBee->GetGameObject()->GetComponent<fovy::SpriteRenderer>();

    spriteRenderer->PlayAnimation(std::string("idle_") +
                                  (snoBee->GetCurrentDirection().x > 0 ? "right" : snoBee->GetCurrentDirection().x < 0 ? "left" : snoBee->GetCurrentDirection().y > 0 ? "down" : "up"));

}

void pengo::SnoBeeIdleState::Exit(pengo::SnoBeeComponent*) {
}

std::unique_ptr<pengo::SnoBeeState> pengo::SnoBeeIdleState::Update(pengo::SnoBeeComponent*) {
    return nullptr;
}

std::unique_ptr<pengo::SnoBeeState> pengo::SnoBeeIdleState::OnMove(pengo::SnoBeeComponent* snoBee, glm::ivec2) {

    if (!snoBee->IsCaught()) {
        return std::make_unique<SnoBeeMoveState>();
    }
    return nullptr;
}

std::unique_ptr<pengo::SnoBeeState> pengo::SnoBeeIdleState::OnPush(pengo::SnoBeeComponent*) {
    return nullptr;
}

std::unique_ptr<pengo::SnoBeeState> pengo::SnoBeeIdleState::OnBreak(pengo::SnoBeeComponent* snoBee) {
    auto direction = snoBee->GetCurrentDirection();
    // snoBee->GetGameObject()->GetComponent<fovy::SpriteRenderer>()->PlayAnimation(
    //     std::string("break_") + (direction.x > 0 ? "right" : direction.x < 0 ? "left" : direction.y > 0 ? "down" : "up"));
    //

    auto toBreakTile = snoBee->GetGrid()->GridPositionFromWorld(snoBee->GetGameObject()->GetTransform().GetWorldPosition()) +
        glm::ivec2{static_cast<int>(direction.x), static_cast<int>(direction.y)};

    if (snoBee->GetGrid()->IsWithinBounds(toBreakTile) && snoBee->GetGrid()->IsOccupied(toBreakTile)) {
        auto* iceBlock = snoBee->GetGrid()->GetCell(toBreakTile).occupant;
        if (iceBlock && iceBlock->HasComponent<pengo::IceBlockComponent>()) {
            iceBlock->GetComponent<pengo::IceBlockComponent>()->Break();
            return std::make_unique<pengo::SnoBeeIdleState>();
        }
    }

    return std::make_unique<pengo::SnoBeeIdleState>();
}

void pengo::SnoBeeMoveState::Enter(pengo::SnoBeeComponent* snoBee) {
    auto* gridComponent = snoBee->GetGrid();
    const glm::vec2 dir = snoBee->GetCurrentDirection();

    auto gridPosition = gridComponent->GridPositionFromWorld(snoBee->GetGameObject()->GetTransform().GetWorldPosition());
    auto nextGridPosition = gridPosition + glm::ivec2{static_cast<int>(dir.x), static_cast<int>(dir.y)};

    m_isValidMove = gridComponent->IsWithinBounds(nextGridPosition) && !gridComponent->IsOccupied(nextGridPosition);
    m_TargetPosition = gridComponent->WorldPositionFromGrid(nextGridPosition);
    m_StartPosition = snoBee->GetGameObject()->GetTransform().GetWorldPosition();

    m_spriteRenderer = snoBee->GetGameObject()->GetComponent<fovy::SpriteRenderer>();
}

void pengo::SnoBeeMoveState::Exit(pengo::SnoBeeComponent*) {
}

std::unique_ptr<pengo::SnoBeeState> pengo::SnoBeeMoveState::Update(pengo::SnoBeeComponent* snoBee) {
    if (!m_isValidMove) {
        return std::make_unique<SnoBeeIdleState>();
    }

    m_moveTimer += static_cast<float>(fovy::Time::GetInstance().DeltaTime());
    snoBee->GetGameObject()->GetTransform().SetWorldPosition(glm::vec3(
        glm::mix(m_StartPosition, m_TargetPosition, m_moveTimer / m_moveDuration), 0));


    m_spriteRenderer->PlayAnimation(std::string("walk_") + (snoBee->GetCurrentDirection().x > 0 ? "right" : snoBee->GetCurrentDirection().x < 0 ? "left" : snoBee->GetCurrentDirection().y > 0 ? "down" : "up"));


    if (m_moveTimer >= m_moveDuration) {
        snoBee->GetGameObject()->GetTransform().SetWorldPosition(glm::vec3(m_TargetPosition, 0.0f));
        return std::make_unique<SnoBeeIdleState>();
    }

    if (m_moveTimer / m_moveDuration >= 0.5f && !m_updatedGrid) {
        auto* grid = snoBee->GetGrid();
        const glm::ivec2 oldGridPosition = grid->GridPositionFromWorld(glm::vec3(m_StartPosition, 0.0f));
        grid->SetOccupant(oldGridPosition, nullptr);

        const auto newGridPosition = grid->GridPositionFromWorld(glm::vec3(m_TargetPosition, 0.0f));
        grid->SetOccupant(newGridPosition, snoBee->GetGameObject());
        m_updatedGrid = true;
    }

    return nullptr;
}

std::unique_ptr<pengo::SnoBeeState> pengo::SnoBeeMoveState::OnMove(pengo::SnoBeeComponent*, glm::ivec2) {
    return nullptr;
}

std::unique_ptr<pengo::SnoBeeState> pengo::SnoBeeMoveState::OnPush(pengo::SnoBeeComponent*) {
    return nullptr;
}
