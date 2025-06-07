#include "IceblockStates.h"

#include <stdexcept>

#include "Components/SpriteRenderer.h"
#include "ObjectModel/GameObject.h"

void pengo::IceBlockIdleState::Enter(pengo::IceBlockComponent*) {
}

void pengo::IceBlockIdleState::Exit(pengo::IceBlockComponent*) {
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockIdleState::Update(pengo::IceBlockComponent*) {
    return nullptr;
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockIdleState::OnPush(pengo::IceBlockComponent* iceBlock) {
    auto dir = iceBlock->GetSlideDirection();
    auto* grid = iceBlock->GetGrid();
    auto currentGridPos = grid->GridPositionFromWorld(iceBlock->GetGameObject()->GetTransform().GetWorldPosition());
    glm::ivec2 targetPos = currentGridPos + dir;

    if (!grid->IsWithinBounds(targetPos) || grid->IsOccupied(targetPos)) {
        return std::make_unique<pengo::IceBlockBreakState>();
    }

    return std::make_unique<pengo::IceBlockSlidingState>();
}

void pengo::IceBlockSlidingState::Enter(pengo::IceBlockComponent* iceBlock) {
    auto slideDirection = iceBlock->GetSlideDirection();
    auto* grid = iceBlock->GetGrid();
    auto currentGridPos = grid->GridPositionFromWorld(iceBlock->GetGameObject()->GetTransform().GetWorldPosition());

    glm::ivec2 newGridPosition = iceBlock->GetGrid()->GridPositionFromWorld(iceBlock->GetGameObject()->GetTransform().GetWorldPosition());
    newGridPosition += slideDirection;
    if (grid->IsWithinBounds(newGridPosition) && !grid->IsOccupied(newGridPosition)) {
        iceBlock->GetGameObject()->GetTransform().SetWorldPosition(grid->WorldPositionFromGrid(newGridPosition));
        grid->SetOccupant(currentGridPos, nullptr);
        grid->SetOccupant(newGridPosition, iceBlock->GetGameObject());
        m_validMove = true;
    } else {
        throw std::runtime_error("Invalid move for ice block: out of bounds or occupied cell.");
    }
}

void pengo::IceBlockSlidingState::Exit(pengo::IceBlockComponent*) {
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockSlidingState::Update(pengo::IceBlockComponent*) {
    if (!m_validMove) {
        return std::make_unique<pengo::IceBlockIdleState>();
    }

    return std::make_unique<IceBlockIdleState>();
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockSlidingState::OnPush(pengo::IceBlockComponent*) {
    return nullptr;
}

void pengo::IceBlockBreakState::Enter(pengo::IceBlockComponent* comp) {
    comp->GetGameObject()->Destroy();
}

void pengo::IceBlockBreakState::Exit(pengo::IceBlockComponent*) {
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockBreakState::Update(pengo::IceBlockComponent*) {
    return nullptr;
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockBreakState::OnPush(pengo::IceBlockComponent*) {
    return nullptr;
}
