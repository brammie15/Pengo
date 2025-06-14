#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include <array>

#include "Event.h"
#include "GameCommands.h"
#include "Prefabs.h"
#include "Singleton.h"
#include "Components/MainGameController.h"
#include "Components/Pengo/PengoComponent.h"
#include "Components/SnoBee/SnoBeeComponent.h"
#include "Components/Tile/IceBlockComponent.h"
#include "Input/InputManager.h"

namespace pengo {

enum class GameMode {
    Singleplayer,
    Multiplayer,
    Versus
};

class GameController final: public fovy::Singleton<GameController> {
    friend class fovy::Singleton<GameController>;
public:

    void AddScore(int score) {
        m_score += score;
        m_onScoreChangedEvent.Invoke(m_score);
    }

    [[nodiscard]] int GetScore() const {
        return m_score;
    }

    void ResetScore() {
        m_score = 0;
        m_onScoreChangedEvent.Invoke(m_score);
    }

    fovy::Event<int>& GetOnScoreChangedEvent() {
        return m_onScoreChangedEvent;
    }

    GameMode GetGameMode() const {
        return m_gameMode;
    }
    void SetGameMode(GameMode mode) {
        m_gameMode = mode;
    }

    // void SetupBindings(PengoComponent* pengo, SnoBeeComponent*) {
    //     const std::array KeyboardInputs{
    //         fovy::InputAction{{SDL_SCANCODE_W, SDL_SCANCODE_UP}, {SDL_CONTROLLER_BUTTON_DPAD_UP}},
    //         fovy::InputAction{{SDL_SCANCODE_A, SDL_SCANCODE_LEFT}, {{SDL_CONTROLLER_BUTTON_DPAD_LEFT}}},
    //         fovy::InputAction{{SDL_SCANCODE_S, SDL_SCANCODE_DOWN}, {{SDL_CONTROLLER_BUTTON_DPAD_DOWN}}},
    //         fovy::InputAction{{SDL_SCANCODE_D, SDL_SCANCODE_RIGHT}, {{SDL_CONTROLLER_BUTTON_DPAD_RIGHT}}},
    //     };
    //
    //     constexpr std::array Directions{
    //         fovy::Direction::Up,
    //         fovy::Direction::Left,
    //         fovy::Direction::Down,
    //         fovy::Direction::Right
    //     };
    //
    //     for (int index{ 0 }; index < KeyboardInputs.size(); ++index) {
    //         const auto& input = KeyboardInputs[index];
    //         auto dir = Directions[index];
    //
    //         fovy::InputManager::GetInstance().AddCommand<PlayerMoveCommand>(
    //             input,
    //             fovy::ButtonState::Down,
    //             pengo,
    //             dir
    //         );
    //
    //         fovy::InputManager::GetInstance().AddCommand<PlayerMoveCommand>(
    //             input,
    //             fovy::ButtonState::Released,
    //             pengo,
    //             fovy::Direction::None
    //         );
    //     }
    //
    //     fovy::InputAction pushInput{{SDL_SCANCODE_SPACE}, {SDL_CONTROLLER_BUTTON_A}};
    //     fovy::InputManager::GetInstance().AddCommand<PlayerPushCommand>(pushInput, fovy::ButtonState::Pressed, pengo);
    // }

    void SetGameStarted(bool started) {
        if (!m_gameStarted) {
            m_gameStarted = started;
        }
        //we can only start the game once
    }

    [[nodiscard]] bool IsGameStarted() const {
        return m_gameStarted;
    }

    MainGameController* GetMainGameController() {
        return m_mainGameController;
    }

    void SetMainGameController(MainGameController* mainGameController) {
        m_mainGameController = mainGameController;
    }

    void ResetGame() {
        m_gameStarted = false;
        m_score = 0;
        m_onScoreChangedEvent.Invoke(m_score);
        if (m_mainGameController) {
            // m_mainGameController->ResetGame();
        }
    }

    //Forgive me for my sins
    PengoComponent* GetPengo() const {
        if (m_mainGameController) {
            return m_mainGameController->GetPengo();
        }
        return nullptr;
    }

    void enemyDied() {
        --m_enemiesRemaining;
        if (m_enemiesRemaining <= 0 && m_mainGameController->GetInfestedTiles().size() == 0) {
            m_mainGameController->OnAllEnemiesDefeated();
        }

        // auto infestedTiles = m_mainGameController->GetInfestedTiles();
        // auto gridPos = infestedTiles[0]->GetGameObject()->GetTransform().GetWorldPosition();
        // glm::ivec2 realGridPos = m_mainGameController->GetGrid()->GridPositionFromWorld(gridPos);
        // infestedTiles[0]->Destroy();
        // AddEnemy(fovy::SceneManager::GetInstance().GetScenes()[1].get(), m_mainGameController->GetGrid(), realGridPos, "SnoBee1");

    }

    void RemoveSnoBee(fovy::GameObject* snoBee);
    void PlayerDied() {
        if (m_mainGameController) {
            m_mainGameController->OnPlayerDied();
        }
    }

private:
    GameController() = default;

    fovy::Event<int> m_onScoreChangedEvent;

    //Kinda bad
    MainGameController* m_mainGameController{nullptr};

    GameMode m_gameMode{GameMode::Singleplayer};
    bool m_gameStarted{false};
    int m_score{0};

    int m_enemiesRemaining{3};
};
}



#endif //GAMECONTROLLER_H
