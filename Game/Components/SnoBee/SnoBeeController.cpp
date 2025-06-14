
#include "SnoBeeController.h"

#include "GameController.h"
#include "SnoBeeComponent.h"
#include "Timer.h"
#include "ObjectModel/GameObject.h"

namespace pengo {

SnoBeeController::SnoBeeController(fovy::GameObject& parent, GridComponent* grid)
    : Component(parent, "AIController"), m_pGrid(grid) 
{
}

void SnoBeeController::Update() {
    float deltaTime = static_cast<float>(fovy::Time::GetInstance().DeltaTime());
    
    // Update timers based on current state
    if (m_currentState == AIState::Normal) {
        m_rageTimer += deltaTime;
        if (m_rageTimer >= m_rageTimeLimit) {
            EnterFrenzyMode();
        }
    } else if (m_currentState == AIState::Frenzy) {
        m_frenzyTimer += deltaTime;
        if (m_frenzyTimer >= m_frenzyDuration) {
            m_currentState = AIState::Normal;
            m_rageTimer = 0.0f; // Reset rage timer
        }
    }

    m_decisionTimer += deltaTime;
    
    if (m_decisionTimer >= m_decisionInterval) {
        m_decisionTimer = 0.0f;
        UpdateAI();
    }
    
    if (!m_commandQueue.empty()) {
        auto& currentCommand = m_commandQueue.front();
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
    // Clear any existing commands
    while (!m_commandQueue.empty()) {
        m_commandQueue.pop();
    }
}

void SnoBeeController::UpdateAI() {
    auto* snoBee = GetGameObject()->GetComponent<SnoBeeComponent>();
    if (!snoBee) return;

    if (!GameController::GetInstance().IsGameStarted()) return;

    glm::ivec2 currentPos = m_pGrid->GridPositionFromWorld(GetGameObject()->GetTransform().GetWorldPosition());
    
    if (m_currentState == AIState::Frenzy) {
        // In frenzy mode - break blocks in current direction
        glm::vec2 currentDirVec = snoBee->GetCurrentDirection();
        fovy::Direction currentDirection = 
            currentDirVec.x > 0 ? fovy::Direction::Right :
            currentDirVec.x < 0 ? fovy::Direction::Left :
            currentDirVec.y > 0 ? fovy::Direction::Down : fovy::Direction::Up;

        std::string direction =
            currentDirection == fovy::Direction::Right ? "right" :
            currentDirection == fovy::Direction::Left ? "left" :
            currentDirection == fovy::Direction::Down ? "down" : "up";

        snoBee->GetGameObject()->GetComponent<fovy::SpriteRenderer>()->PlayAnimation("frenzy_" + direction);

        
        // Try to break block in current direction
        m_commandQueue.push(std::make_unique<BreakCommand>(currentDirection));
        m_commandQueue.push(std::make_unique<MoveCommand>(currentDirection));
        return;
    }

    // Check if stuck (no valid moves)
    if (IsStuck(currentPos)) {
        EnterFrenzyMode();
        return;
    }

    // Normal behavior
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
    
    for (auto dir : directions) {
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
    fovy::Direction currentDirection = 
        currentDirVec.x > 0 ? fovy::Direction::Right :
        currentDirVec.x < 0 ? fovy::Direction::Left :
        currentDirVec.y > 0 ? fovy::Direction::Down : fovy::Direction::Up;
    
    // Check if we're in a corridor
    if (IsInCorridor(currentPos, currentDirection)) {
        // In corridor - keep moving in current direction if possible
        if (IsValidMove(currentPos, currentDirection)) {
            return currentDirection;
        }
        
        // Current direction blocked, find the only other possible direction
        fovy::Direction directions[] = {
            fovy::Direction::Right,
            fovy::Direction::Left,
            fovy::Direction::Up,
            fovy::Direction::Down
        };
        
        for (auto dir : directions) {
            if (dir != currentDirection && IsValidMove(currentPos, dir)) {
                return dir;
            }
        }
    }
    
    // At intersection (more than 2 possible directions) - move toward target
    if (m_pTarget != nullptr) {
        glm::ivec2 targetPos = m_pGrid->GridPositionFromWorld(m_pTarget->GetTransform().GetWorldPosition());
        glm::ivec2 diff = targetPos - currentPos;
        
        // Try to move in direction that reduces largest distance component
        fovy::Direction bestDirection = fovy::Direction::None;
        int bestDistance = INT_MAX;
        
        fovy::Direction directions[] = {
            fovy::Direction::Right,
            fovy::Direction::Left,
            fovy::Direction::Up,
            fovy::Direction::Down
        };
        
        for (auto dir : directions) {
            if (IsValidMove(currentPos, dir)) {
                glm::ivec2 dirVec{0, 0};
                switch (dir) {
                    case fovy::Direction::Up:    dirVec = {0, -1}; break;
                    case fovy::Direction::Down:  dirVec = {0, 1}; break;
                    case fovy::Direction::Left:  dirVec = {-1, 0}; break;
                    case fovy::Direction::Right: dirVec = {1, 0}; break;
                    default: break;
                }
                
                glm::ivec2 newPos = currentPos + dirVec;
                int newDistance = abs(targetPos.x - newPos.x) + abs(targetPos.y - newPos.y);
                
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
    
    // If no target or all paths blocked, try random valid direction
    fovy::Direction directions[] = {
        fovy::Direction::Right,
        fovy::Direction::Left,
        fovy::Direction::Up,
        fovy::Direction::Down
    };
    
    for (auto dir : directions) {
        if (IsValidMove(currentPos, dir)) {
            return dir;
        }
    }
    
    return fovy::Direction::None;
}

bool SnoBeeController::IsValidMove(const glm::ivec2& position, fovy::Direction direction) const {
    glm::ivec2 dirVec{0, 0};
    
    switch (direction) {
        case fovy::Direction::Up:    dirVec = {0, -1}; break;
        case fovy::Direction::Down:  dirVec = {0, 1}; break;
        case fovy::Direction::Left:  dirVec = {-1, 0}; break;
        case fovy::Direction::Right: dirVec = {1, 0}; break;
        default: return false;
    }
    
    glm::ivec2 newPos = position + dirVec;
    return m_pGrid->IsWithinBounds(newPos) && !m_pGrid->IsOccupied(newPos);
}

} //