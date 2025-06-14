#include "MainGameController.h"

#include <map>

#include "GameController.h"
#include "Prefabs.h"
#include "ServiceLocator.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

pengo::MainGameController::MainGameController(fovy::GameObject& pParent, PengoComponent* pengo, GridComponent* grid, SnoBeeComponent* snoBee, const std::map<fovy::Direction, fovy::GameObject*>& walls):
    Component{pParent, "MainGameController"}, m_pengo{pengo}, m_grid{grid}, m_snoBee{snoBee}, m_walls{walls} {
}

void pengo::MainGameController::Update() {
}

void pengo::MainGameController::SetupBindings(GridComponent* grid) {
    const std::array KeyboardInputs{
        fovy::InputAction{{SDL_SCANCODE_W, SDL_SCANCODE_UP}, {SDL_CONTROLLER_BUTTON_DPAD_UP}},
        fovy::InputAction{{SDL_SCANCODE_A, SDL_SCANCODE_LEFT}, {{SDL_CONTROLLER_BUTTON_DPAD_LEFT}}},
        fovy::InputAction{{SDL_SCANCODE_S, SDL_SCANCODE_DOWN}, {{SDL_CONTROLLER_BUTTON_DPAD_DOWN}}},
        fovy::InputAction{{SDL_SCANCODE_D, SDL_SCANCODE_RIGHT}, {{SDL_CONTROLLER_BUTTON_DPAD_RIGHT}}},
    };

    const std::array ControllerInputs{
        fovy::InputAction{{}, {SDL_CONTROLLER_BUTTON_DPAD_UP},},
        fovy::InputAction{{}, {SDL_CONTROLLER_BUTTON_DPAD_LEFT}},
        fovy::InputAction{{}, {SDL_CONTROLLER_BUTTON_DPAD_DOWN}},
        fovy::InputAction{{}, {SDL_CONTROLLER_BUTTON_DPAD_RIGHT}}
    };

    constexpr std::array Directions{
        fovy::Direction::Up,
        fovy::Direction::Left,
        fovy::Direction::Down,
        fovy::Direction::Right
    };

    auto currentGameMode = GameController::GetInstance().GetGameMode();
    if (currentGameMode == GameMode::Singleplayer || currentGameMode == GameMode::Versus || currentGameMode == GameMode::Multiplayer) {
        for (int index{0}; index < KeyboardInputs.size(); ++index) {
            const auto& input = KeyboardInputs[index];
            auto dir = Directions[index];
            fovy::InputManager::GetInstance().AddCommand<PlayerMoveCommand>(
                input,
                fovy::ButtonState::Released,
                m_pengo,
                fovy::Direction::None
            );

            fovy::InputManager::GetInstance().AddCommand<PlayerMoveCommand>(
                input,
                fovy::ButtonState::Down,
                m_pengo,
                dir
            );
        }

        fovy::InputManager::GetInstance().AddCommand<PlayerPushCommand>(
            fovy::InputAction{{SDL_SCANCODE_SPACE}, {SDL_CONTROLLER_BUTTON_A}},
            fovy::ButtonState::Pressed,
            m_pengo
        );
    }

    if (currentGameMode == GameMode::Multiplayer) {
        fovy::Scene& scene = fovy::SceneManager::GetInstance().GetCurrentScene();

        //Random location
        bool foundSpawn{false};
        glm::ivec2 spawnPos{0, 0};
        while (!foundSpawn) {
            glm::ivec2 randomPos{rand() % grid->GetWidth(), rand() % grid->GetHeight()};
            if (!grid->IsOccupied(randomPos)) {
                spawnPos = randomPos;
                foundSpawn = true;
            }
        }


        //Add 2nd player
        auto playerComponent = pengo::CreatePlayer(scene, grid, "Player2", spawnPos);

        playerComponent->GetWallPushEvent().AddListener([this] (fovy::Direction dir) {
            if (dir != fovy::Direction::Up) return;
            m_walls[fovy::Direction::Up]->GetComponent<fovy::SpriteRenderer>()->PlayAnimation("shake", true);
            fovy::ServiceLocator<fovy::ISoundSystem>::GetService().PlayAsync("Data/Sounds/push_wall.wav", 0.2f, 0);
        });
        playerComponent->GetWallPushEvent().AddListener([this] (fovy::Direction dir) {
            if (dir != fovy::Direction::Down) return;
            m_walls[fovy::Direction::Down]->GetComponent<fovy::SpriteRenderer>()->PlayAnimation("shake", true);
            fovy::ServiceLocator<fovy::ISoundSystem>::GetService().PlayAsync("Data/Sounds/push_wall.wav", 0.2f, 0);
        });
        playerComponent->GetWallPushEvent().AddListener([this] (fovy::Direction dir) {
            if (dir != fovy::Direction::Left) return;
            m_walls[fovy::Direction::Left]->GetComponent<fovy::SpriteRenderer>()->PlayAnimation("shake", true);
            fovy::ServiceLocator<fovy::ISoundSystem>::GetService().PlayAsync("Data/Sounds/push_wall.wav", 0.2f, 0);
        });
        playerComponent->GetWallPushEvent().AddListener([this] (fovy::Direction dir) {
            if (dir != fovy::Direction::Right) return;
            m_walls[fovy::Direction::Right]->GetComponent<fovy::SpriteRenderer>()->PlayAnimation("shake", true);
            fovy::ServiceLocator<fovy::ISoundSystem>::GetService().PlayAsync("Data/Sounds/push_wall.wav", 0.2f, 0);
        });

        for (int index{0}; index < ControllerInputs.size(); ++index) {
            const auto& input = ControllerInputs[index];
            auto dir = Directions[index];
            fovy::InputManager::GetInstance().AddCommand<PlayerMoveCommand>(
                input,
                fovy::ButtonState::Released,
                1,
                playerComponent,
                fovy::Direction::None
            );

            fovy::InputManager::GetInstance().AddCommand<PlayerMoveCommand>(
                input,
                fovy::ButtonState::Down,
                1,
                playerComponent,
                dir
            );
        }

        fovy::InputManager::GetInstance().AddCommand<PlayerPushCommand>(
            fovy::InputAction{{}, {SDL_CONTROLLER_BUTTON_A}},
            fovy::ButtonState::Pressed,
            1,
            playerComponent
        );

        // scene.Add(Player1);
    }
}
