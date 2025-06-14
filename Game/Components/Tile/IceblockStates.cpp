#include "IceblockStates.h"

#include <iostream>

#include "GameController.h"
#include "Prefabs.h"
#include "ServiceLocator.h"
#include "Timer.h"
#include "../../../Minigin/Components/SpriteRenderer.h"
#include "Components/SnoBee/SnoBeeComponent.h"
#include "Components/Tile/IceBlockComponent.h"
#include "ObjectModel/GameObject.h"
#include "Scene/SceneManager.h"

void pengo::IceBlockSpawnState::Enter(pengo::IceBlockComponent* iceBlock) {
    if (iceBlock->HasEgg()) {
        m_shouldFlash = true;
        auto* spriteRenderer = iceBlock->GetGameObject()->GetComponent<fovy::SpriteRenderer>();
        spriteRenderer->PlayAnimation("egg_flash");
        m_flashDuration = iceBlock->GetGameObject()->GetComponent<fovy::SpriteRenderer>()->GetCurrentAnimationDuration();
    }
}

void pengo::IceBlockSpawnState::Exit(pengo::IceBlockComponent* iceBlock) {
    if (iceBlock->HasEgg()) {
        iceBlock->GetGameObject()->GetComponent<fovy::SpriteRenderer>()->SetTileIndex(0);
    }
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockSpawnState::Update(pengo::IceBlockComponent*) {
    if (m_shouldFlash) {
        m_flashCounter += static_cast<float>(fovy::Time::GetInstance().DeltaTime());
        if (m_flashCounter >= m_flashDuration) {
            m_shouldFlash = false;
            m_flashCounter = 0.0f;

            // After flashing, switch to idle state
            GameController::GetInstance().SetGameStarted(true);
            return std::make_unique<IceBlockIdleState>();
        }
    } else {
        return std::make_unique<IceBlockIdleState>();
    }

    return nullptr;
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockSpawnState::OnPush(pengo::IceBlockComponent*) {
    return nullptr; // Cannot push while spawning
}

void pengo::IceBlockIdleState::Enter(pengo::IceBlockComponent*) {
}

void pengo::IceBlockIdleState::Exit(pengo::IceBlockComponent*) {
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockIdleState::Update(pengo::IceBlockComponent*) {
    return nullptr;
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockIdleState::OnPush(pengo::IceBlockComponent* iceBlock) {
    const auto dir = iceBlock->GetSlideDirection();
    auto* grid = iceBlock->GetGrid();
    const auto currentGridPos = grid->GridPositionFromWorld(iceBlock->GetGameObject()->GetTransform().GetWorldPosition());
    const glm::ivec2 targetPos = currentGridPos + dir;

    if (!grid->IsWithinBounds(targetPos)) {
        //Pushing into wall, break
        return std::make_unique<IceBlockBreakState>();
    }


    if (grid->IsOccupied(targetPos)) {
        fovy::GameObject* targetOccupant = grid->GetCell(targetPos).occupant;
        const bool isSnobee = targetOccupant && targetOccupant->HasComponent<pengo::SnoBeeComponent>();

        if (isSnobee) {
            return std::make_unique<pengo::IceBlockSlidingState>();
        }
        if (!iceBlock->IsDiamond()) {
            return std::make_unique<pengo::IceBlockBreakState>();
        } else {
            return nullptr;
        }
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
    const auto nextNextPos = nextPos + m_slideDirection;
    m_targetPos = m_grid->WorldPositionFromGrid(nextPos);

    fovy::GameObject* nextOccupant = m_grid->GetCell(nextPos).occupant;
    if (nextOccupant != nullptr && nextOccupant->HasComponent<pengo::SnoBeeComponent>()) {
        if (!m_grid->IsWithinBounds(nextNextPos) || m_grid->IsOccupied(nextNextPos)) {
            nextOccupant->Destroy();
            GameController::GetInstance().enemyDied();
            auto* snoBeeComponent = nextOccupant->GetComponent<pengo::SnoBeeComponent>();
            snoBeeComponent->SetCaught(true);
            m_attachedEnemy = nullptr;
            GameController::GetInstance().AddScore(100);

            SpawnPointsCollected(fovy::SceneManager::GetInstance().GetCurrentScene(), glm::vec3(m_targetPos, 0.0f), 100);


            return; // SnoBee is at the edge of the grid, cannot attach
        }

        m_attachedEnemy = nextOccupant;
        nextOccupant->GetTransform().SetParent(&go->GetTransform());
    } else {
        m_attachedEnemy = nullptr;
    }
}

void pengo::IceBlockSlidingState::Exit(IceBlockComponent* block) {
    if (!block->IsDiamond()) {
        return;
    }

    auto* grid = block->GetGrid();
    const auto currentGridPos = grid->GridPositionFromWorld(block->GetGameObject()->GetTransform().GetWorldPosition());

    constexpr glm::ivec2 directions[] = {
        {1, 0}, // right
        {0, 1}, // up
        {-1, 0}, // left
        {0, -1} // down
    };

    for (const auto& dir: directions) {
        // Check if there are two more diamond blocks in this direction
        const glm::ivec2 pos1 = currentGridPos + dir;
        const glm::ivec2 pos2 = currentGridPos - dir;

        const bool hasLeft = grid->IsWithinBounds(pos1) && grid->IsOccupied(pos1);
        const bool hasRight = grid->IsWithinBounds(pos2) && grid->IsOccupied(pos2);

        if (hasLeft && hasRight) {
            auto* leftBlock = grid->GetCell(pos1).occupant->GetComponent<IceBlockComponent>();
            auto* rightBlock = grid->GetCell(pos2).occupant->GetComponent<IceBlockComponent>();

            if (leftBlock && leftBlock->IsDiamond() &&
                rightBlock && rightBlock->IsDiamond()) {
                leftBlock->Break();
                rightBlock->Break();
                block->Break();

                GameController::GetInstance().AddScore(500);
                SpawnPointsCollected(
                    fovy::SceneManager::GetInstance().GetCurrentScene(), block->GetTransform().GetWorldPosition(), 500
                );
                break; // No need to check other directions
            }
        }
    }
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
        const glm::ivec2 nextNextGrid = nextGrid + m_slideDirection;

        if (m_attachedEnemy != nullptr) {
            if (!m_grid->IsWithinBounds(nextNextGrid) || (m_grid->IsOccupied(nextNextGrid) || !m_grid->IsWithinBounds(nextNextGrid))) {
                m_attachedEnemy->Destroy();
                GameController::GetInstance().AddScore(100);
                GameController::GetInstance().enemyDied();
                auto* snoBeeComponent = m_attachedEnemy->GetComponent<pengo::SnoBeeComponent>();
                snoBeeComponent->SetCaught(true);
                m_attachedEnemy = nullptr;
                SpawnPointsCollected(fovy::SceneManager::GetInstance().GetCurrentScene(), glm::vec3(m_targetPos, 0.0f), 100);
            }

            if (m_grid->IsWithinBounds(nextNextGrid)) {
                this->PrepareSlide(nextGrid);
                return nullptr;
            }
        }


        if (!m_grid->IsWithinBounds(nextGrid)) {
            //Hitting wall Stop
            return std::make_unique<IceBlockIdleState>();
        }

        fovy::GameObject* nextOccupant = m_grid->GetCell(nextGrid).occupant;
        if (nextOccupant != nullptr) {
            if (nextOccupant->HasComponent<SnoBeeComponent>()) {
                m_attachedEnemy = nextOccupant;
                nextOccupant->GetTransform().SetParent(&iceBlock->GetTransform());

                this->PrepareSlide(nextGrid);
                return nullptr;
            }
        }
        if (!m_grid->IsOccupied(nextGrid)) {
            this->PrepareSlide(nextGrid);
            return nullptr;
        }

        return std::make_unique<IceBlockIdleState>();
    }

    return nullptr;
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockSlidingState::OnPush(IceBlockComponent*) {
    return nullptr;
}

void pengo::IceBlockSlidingState::PrepareSlide(const glm::ivec2& nextGrid) {
    m_slideTimer = 0.0f;
    m_updatedGrid = false;
    m_startPos = m_targetPos;
    m_targetPos = m_grid->WorldPositionFromGrid(nextGrid);
}

void pengo::IceBlockEggBreakState::Enter(pengo::IceBlockComponent* iceBlock) {

    iceBlock->GetGameObject()->GetComponent<fovy::SpriteRenderer>()->PlayAnimation("egg_break");
    m_breakDuration = iceBlock->GetGameObject()->GetComponent<fovy::SpriteRenderer>()->GetCurrentAnimationDuration();
}

void pengo::IceBlockEggBreakState::Exit(pengo::IceBlockComponent*) {
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockEggBreakState::Update(pengo::IceBlockComponent* iceBlock) {
    m_breakTimer += static_cast<float>(fovy::Time::GetInstance().DeltaTime());
    if (m_breakTimer >= m_breakDuration) {

        auto* grid = iceBlock->GetGrid();
        const auto currentGridPos = grid->GridPositionFromWorld(iceBlock->GetGameObject()->GetTransform().GetWorldPosition());
        grid->SetOccupant(currentGridPos, nullptr);
        iceBlock->GetGameObject()->Destroy();


        GameController::GetInstance().AddScore(500);
        SpawnPointsCollected(fovy::SceneManager::GetInstance().GetCurrentScene(), iceBlock->GetGameObject()->GetTransform().GetWorldPosition(), 500);

        GameController::GetInstance().GetMainGameController()->RemoveInfestedTile(iceBlock);
    }

    return nullptr;
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockEggBreakState::OnPush(pengo::IceBlockComponent*) {
    return nullptr;
}

void pengo::IceBlockBreakState::Enter(pengo::IceBlockComponent* iceBlock) {
    if (iceBlock->HasEgg()) {
        m_isEgg = true;
    }

    auto* spriteRenderer = iceBlock->GetGameObject()->GetComponent<fovy::SpriteRenderer>();
    if (spriteRenderer) {
        spriteRenderer->PlayAnimation("break");
        m_breakDuration = spriteRenderer->GetCurrentAnimationDuration();
    }
    fovy::ServiceLocator<fovy::ISoundSystem>::GetService().PlayAsync("Data/Sounds/Ice Block Destroyed.wav", 0.2f, 0);
}

void pengo::IceBlockBreakState::Exit(pengo::IceBlockComponent*) {
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockBreakState::Update(pengo::IceBlockComponent* iceBlock) {
    if (m_isEgg) {
        return std::make_unique<IceBlockEggBreakState>();
    }

    m_breakTimer += static_cast<float>(fovy::Time::GetInstance().DeltaTime());
    if (m_breakTimer >= m_breakDuration) {
        auto* grid = iceBlock->GetGrid();
        const auto currentGridPos = grid->GridPositionFromWorld(iceBlock->GetGameObject()->GetTransform().GetWorldPosition());
        grid->SetOccupant(currentGridPos, nullptr);
        iceBlock->GetGameObject()->Destroy();


        GameController::GetInstance().AddScore(50);
        SpawnPointsCollected(fovy::SceneManager::GetInstance().GetCurrentScene(), iceBlock->GetGameObject()->GetTransform().GetWorldPosition(), 50);

        return nullptr;
    }

    return nullptr;
}

std::unique_ptr<pengo::IceBlockState> pengo::IceBlockBreakState::OnPush(pengo::IceBlockComponent*) {
    return nullptr;
}
