#include "IceblockStates.h"

#include "Timer.h"
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
    const auto dir = iceBlock->GetSlideDirection();
    const auto* grid = iceBlock->GetGrid();
    const auto currentGridPos = grid->GridPositionFromWorld(iceBlock->GetGameObject()->GetTransform().GetWorldPosition());
    const glm::ivec2 targetPos = currentGridPos + dir;

    if (!grid->IsWithinBounds(targetPos) || grid->IsOccupied(targetPos)) {
        return std::make_unique<pengo::IceBlockBreakState>();
    }

    return std::make_unique<pengo::IceBlockSlidingState>();
}

void pengo::IceBlockSlidingState::Enter(IceBlockComponent* iceBlock) {
    m_slideDirection = iceBlock->GetSlideDirection();
    m_grid = iceBlock->GetGrid();
    m_iceBlock = iceBlock;

    auto* go = iceBlock->GetGameObject();
    m_startPos = go->GetTransform().GetWorldPosition();
    const auto currentGridPos = m_grid->GridPositionFromWorld(glm::vec3(m_startPos, 0.0f));
    const auto nextPos = currentGridPos + m_slideDirection;

    m_targetPos = m_grid->WorldPositionFromGrid(nextPos);
}

void pengo::IceBlockSlidingState::Exit(IceBlockComponent*) {
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockSlidingState::Update(IceBlockComponent* iceBlock) {
    m_slideTimer += static_cast<float>(fovy::Time::GetInstance().DeltaTime());

    const float t = glm::clamp(m_slideTimer / m_slideDuration, 0.0f, 1.0f);
    auto* go = iceBlock->GetGameObject();
    go->GetTransform().SetWorldPosition(glm::vec3(glm::mix(m_startPos, m_targetPos, t), 0.0f));

    if (t >= 0.5f && !m_updatedGrid) {
        const glm::ivec2 prevGrid = m_grid->GridPositionFromWorld(glm::vec3(m_startPos, 0.0f));
        const glm::ivec2 nextGrid = m_grid->GridPositionFromWorld(glm::vec3(m_targetPos, 0.0f));
        m_grid->SetOccupant(prevGrid, nullptr);
        m_grid->SetOccupant(nextGrid, go);
        m_updatedGrid = true;
    }

    if (t >= 1.0f) {
        const glm::ivec2 currentGrid = m_grid->GridPositionFromWorld(glm::vec3(m_targetPos, 0.0f));
        const glm::ivec2 nextGrid = currentGrid + m_slideDirection;

        if (m_grid->IsWithinBounds(nextGrid) && !m_grid->IsOccupied(nextGrid)) {
            m_slideTimer = 0.0f;
            m_updatedGrid = false;
            m_startPos = m_targetPos;
            m_targetPos = m_grid->WorldPositionFromGrid(nextGrid);
            return nullptr;
        }

        return std::make_unique<IceBlockIdleState>();
    }

    return nullptr;
}
std::unique_ptr<pengo::IceBlockState> pengo::IceBlockSlidingState::OnPush(IceBlockComponent*) {
    return nullptr;
}

void pengo::IceBlockBreakState::Enter(pengo::IceBlockComponent* iceBlock) {
    auto* spriteRenderer = iceBlock->GetGameObject()->GetComponent<fovy::SpriteRenderer>();
    if (spriteRenderer) {
        spriteRenderer->PlayAnimation("break");
        m_breakDuration = spriteRenderer->GetCurrentAnimationDuration();
    }
    // iceBlock->GetGameObject()->Destroy();
}

void pengo::IceBlockBreakState::Exit(pengo::IceBlockComponent*) {
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockBreakState::Update(pengo::IceBlockComponent* iceBlock) {
    m_breakTimer += static_cast<float>(fovy::Time::GetInstance().DeltaTime());
    if (m_breakTimer >= m_breakDuration) {
        auto* grid = iceBlock->GetGrid();
        const auto currentGridPos = grid->GridPositionFromWorld(iceBlock->GetGameObject()->GetTransform().GetWorldPosition());
        grid->SetOccupant(currentGridPos, nullptr);
        iceBlock->GetGameObject()->Destroy();
        return nullptr;
    }

    return nullptr;
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockBreakState::OnPush(pengo::IceBlockComponent*) {
    return nullptr;
}
