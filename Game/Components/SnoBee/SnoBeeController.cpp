#include "SnoBeeController.h"

#include "GameController.h"
#include "SnoBeeComponent.h"
#include "Timer.h"
#include "ObjectModel/GameObject.h"

namespace pengo {
    SnoBeeController::SnoBeeController(fovy::GameObject& parent, GridComponent* grid)
        : Component(parent, "AIController"), m_pGrid(grid) {
    }

    void SnoBeeController::Update() {
        const float deltaTime = static_cast<float>(fovy::Time::GetInstance().DeltaTime());

        if (m_currentState == AIState::Normal) {
            m_rageTimer += deltaTime;
            if (m_rageTimer >= m_rageTimeLimit) {
                EnterFrenzyMode();
            }
        } else if (m_currentState == AIState::Frenzy) {
            m_frenzyTimer += deltaTime;
            if (m_frenzyTimer >= m_frenzyDuration) {
                m_currentState = AIState::Normal;
                m_rageTimer = 0.0f;
            }
        }

        m_decisionTimer += deltaTime;

        if (m_decisionTimer >= m_decisionInterval) {
            m_decisionTimer = 0.0f;
            UpdateAI();
        }

        if (!m_commandQueue.empty()) {
            const auto& currentCommand = m_commandQueue.front();
            auto* snoBee = GetGameObject()->GetComponent<SnoBeeComponent>();

            if (snoBee) {
                currentCommand->Execute(snoBee);

                if (currentCommand->IsCompleted()) {
                    m_commandQueue.pop();
                }
            }
        }
    }

    void SnoBeeController::EnterFrenzyMode() {
        m_currentState = AIState::Frenzy;
        m_frenzyTimer = 0.0f;
        while (!m_commandQueue.empty()) {
            m_commandQueue.pop();
        }
    }

    void SnoBeeController::UpdateAI() {
        const auto* snoBee = GetGameObject()->GetComponent<SnoBeeComponent>();
        if (!snoBee) return;

        if (!GameController::GetInstance().IsGameStarted()) return;

        const glm::ivec2 currentPos = m_pGrid->GridPositionFromWorld(GetGameObject()->GetTransform().GetWorldPosition());


        glm::ivec2 directions[] = {
            {1, 0},  // Right
            {-1, 0}, // Left
            {0, 1},  // Down
            {0, -1}  // Up
        };
        for (const auto& dir : directions) {
            glm::ivec2 neighborPos = currentPos + dir;
            if (m_pGrid->IsWithinBounds(neighborPos)) {
                auto* occupant = m_pGrid->GetCell(neighborPos).occupant;
                if (occupant && occupant->HasComponent<pengo::PengoComponent>()) {
                    m_commandQueue.push(std::make_unique<MoveCommand>(fovy::VectorToDirection(dir)));
                }
            }
        }


        if (m_currentState == AIState::Frenzy) {
            const glm::vec2 currentDirVec = snoBee->GetCurrentDirection();
            fovy::Direction currentDirection =
                    currentDirVec.x > 0 ? fovy::Direction::Right : currentDirVec.x < 0 ? fovy::Direction::Left : currentDirVec.y > 0 ? fovy::Direction::Down : fovy::Direction::Up;

            const std::string direction =
                    currentDirection == fovy::Direction::Right ? "right" : currentDirection == fovy::Direction::Left ? "left" : currentDirection == fovy::Direction::Down ? "down" : "up";

            snoBee->GetGameObject()->GetComponent<fovy::SpriteRenderer>()->PlayAnimation("frenzy_" + direction);


            m_commandQueue.push(std::make_unique<BreakCommand>(currentDirection));
            m_commandQueue.push(std::make_unique<MoveCommand>(currentDirection));
            return;
        }

        if (IsStuck(currentPos)) {
            EnterFrenzyMode();
            return;
        }

        fovy::Direction nextDirection = CalculateNextMove();
        m_commandQueue.push(std::make_unique<MoveCommand>(nextDirection));
    }

    bool SnoBeeController::IsStuck(const glm::ivec2& position) const {
        return CountPossibleDirections(position) == 0;
    }

    bool SnoBeeController::IsInCorridor(const glm::ivec2& position, fovy::Direction) const {
        return CountPossibleDirections(position) == 2;
    }

    int SnoBeeController::CountPossibleDirections(const glm::ivec2& position) const {
        int count = 0;
        fovy::Direction directions[] = {
            fovy::Direction::Right,
            fovy::Direction::Left,
            fovy::Direction::Up,
            fovy::Direction::Down
        };

        for (auto dir: directions) {
            if (IsValidMove(position, dir)) {
                count++;
            }
        }

        return count;
    }

    fovy::Direction SnoBeeController::CalculateNextMove() {
        const auto* snoBee = GetGameObject()->GetComponent<SnoBeeComponent>();
        if (!snoBee) return fovy::Direction::None;

        glm::ivec2 currentPos = m_pGrid->GridPositionFromWorld(GetGameObject()->GetTransform().GetWorldPosition());
        glm::vec2 currentDirVec = snoBee->GetCurrentDirection();


        if (m_pTarget != nullptr) {
            glm::ivec2 targetPos = m_pGrid->GridPositionFromWorld(m_pTarget->GetTransform().GetWorldPosition());
            glm::ivec2 diff = targetPos - currentPos;

            // Try to move in direction that reduces largest distance component
            auto bestDirection = fovy::Direction::None;
            int bestDistance = INT_MAX;

            fovy::Direction directions[] = {
                fovy::Direction::Right,
                fovy::Direction::Left,
                fovy::Direction::Up,
                fovy::Direction::Down
            };

            for (auto dir: directions) {
                if (IsValidMove(currentPos, dir)) {
                    glm::ivec2 dirVec{0, 0};
                    switch (dir) {
                        case fovy::Direction::Up: dirVec = {0, -1};
                            break;
                        case fovy::Direction::Down: dirVec = {0, 1};
                            break;
                        case fovy::Direction::Left: dirVec = {-1, 0};
                            break;
                        case fovy::Direction::Right: dirVec = {1, 0};
                            break;
                        default: break;
                    }

                    const glm::ivec2 newPos = currentPos + dirVec;
                    const int newDistance = abs(targetPos.x - newPos.x) + abs(targetPos.y - newPos.y);

                    if (newDistance < bestDistance) {
                        bestDistance = newDistance;
                        bestDirection = dir;
                    }
                }
            }

            if (bestDirection != fovy::Direction::None) {
                return bestDirection;
            }
        }

        fovy::Direction directions[] = {
            fovy::Direction::Right,
            fovy::Direction::Left,
            fovy::Direction::Up,
            fovy::Direction::Down
        };

        for (auto dir: directions) {
            if (IsValidMove(currentPos, dir)) {
                return dir;
            }
        }

        return fovy::Direction::None;
    }

    bool SnoBeeController::IsValidMove(const glm::ivec2& position, fovy::Direction direction) const {
        glm::ivec2 dirVec{0, 0};

        switch (direction) {
            case fovy::Direction::Up: dirVec = {0, -1};
                break;
            case fovy::Direction::Down: dirVec = {0, 1};
                break;
            case fovy::Direction::Left: dirVec = {-1, 0};
                break;
            case fovy::Direction::Right: dirVec = {1, 0};
                break;
            default: return false;
        }

        const glm::ivec2 newPos = position + dirVec;


        if (m_pGrid->IsWithinBounds(newPos)) {
            bool isPlayer = m_pGrid->GetCell(newPos).occupant &&
                            m_pGrid->GetCell(newPos).occupant->HasComponent<pengo::PengoComponent>();
            if (isPlayer) {
                return true;
            } else if (m_pGrid->IsOccupied(newPos)) {
                return false;
            } else {
                return true;
            }
        }
        return false;
    }
}
