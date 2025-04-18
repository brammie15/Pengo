#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Minigin.h"
#include "Managers/ResourceManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#include <array>
#include <filesystem>
#include <XInput.h>

#include "Components/FPSComponent.h"
#include "Components/HealthComponent.h"
#include "Components/LivesDisplayComponent.h"
#include "Components/ScoreComponent.h"
#include "Components/ScoreDisplayComponent.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Components/ThrashTheCacheComponent.h"
#include "Input/InputAction.h"
#include "Input/InputBinding.h"
#include "Input/InputManager.h"
#include "Managers/AchievementManager.h"
#include "ObjectModel/GameObject.h"

namespace fs = std::filesystem;


void load() {
    auto& scene = fovy::SceneManager::GetInstance().CreateScene("Demo");

    const auto go = std::make_shared<fovy::GameObject>("Background");
    go->AddComponent<fovy::TextureComponent>(fovy::ResourceManager::GetInstance().LoadTexture("background.tga"));
    scene.Add(go);

    auto font = fovy::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    auto smallerFont = fovy::ResourceManager::GetInstance().LoadFont("Lingua.otf", 22);

    const auto newText = std::make_shared<fovy::GameObject>("HelloWorldText");
    newText->AddComponent<fovy::TextComponent>(std::string("Hello World!"), font);
    newText->GetTransform().SetWorldPosition(glm::vec3{200, 200, 0});

    scene.Add(newText);

    const auto imageTest = std::make_shared<fovy::GameObject>("DaeLogo");
    auto texture = fovy::ResourceManager::GetInstance().LoadTexture("logo.tga");
    imageTest->AddComponent<fovy::TextureComponent>(texture);
    imageTest->GetTransform().SetWorldPosition(glm::vec3{200, 100, 0});

    scene.Add(imageTest);

    const auto fpsGameobject = std::make_shared<fovy::GameObject>("FpsText");
    const auto textComponent = fpsGameobject->AddComponent<fovy::TextComponent>("", font);
    const auto fpsComponent = fpsGameobject->AddComponent<fovy::FPSComponent>();
    fpsComponent->SetTargetTextComponent(textComponent);

    scene.Add(fpsGameobject);

    const std::string playerNames[2] = {"player1.png", "player2.png"};

    using fovy::InputAction;

    std::array<InputAction, 4> KeyboardInputs{
        InputAction{{SDL_SCANCODE_W}},
        InputAction{{SDL_SCANCODE_A}},
        InputAction{{SDL_SCANCODE_S}},
        InputAction{{SDL_SCANCODE_D}},
    };

    std::array<InputAction, 4> ControllerInputs{
        InputAction{{}, {XINPUT_GAMEPAD_DPAD_UP}},
        InputAction{{}, {XINPUT_GAMEPAD_DPAD_LEFT}},
        InputAction{{}, {XINPUT_GAMEPAD_DPAD_DOWN}},
        InputAction{{}, {XINPUT_GAMEPAD_DPAD_RIGHT}},
    };

    std::array<glm::vec3, 4> MoveDirections{
        glm::vec3{0, -1, 0},
        glm::vec3{-1, 0, 0},
        glm::vec3{0, 1, 0},
        glm::vec3{1, 0, 0},
    };

    auto PlayerExplain1 = std::make_shared<fovy::GameObject>();
    PlayerExplain1->GetTransform().SetWorldPosition({0, 300,0});
    PlayerExplain1->AddComponent<fovy::TextComponent>("Player1: WASD to move, C to Damage, Z to Add 100 points", smallerFont);

    scene.Add(PlayerExplain1);

    auto PlayerExplain2 = std::make_shared<fovy::GameObject>();
    PlayerExplain2->GetTransform().SetWorldPosition({0, 350,0});
    PlayerExplain2->AddComponent<fovy::TextComponent>("Player2: D-Pad to move, X to Damage, B to Add 100 points", smallerFont);

    scene.Add(PlayerExplain2);



    float baseHeight = 40;
    for (int index{0}; index < 2; ++index) {
        //PlayerScoreDisplay
        auto Player1Text = std::make_shared<fovy::GameObject>();
        Player1Text->GetTransform().SetWorldPosition({0, baseHeight, 0});
        Player1Text->AddComponent<fovy::TextComponent>(std::string("#Player ") + std::to_string(index + 1), smallerFont);
        baseHeight += 30;

        const auto playerScoreDisplay = std::make_shared<fovy::GameObject>("PlayerScoreDisplay");
        playerScoreDisplay->GetTransform().SetWorldPosition({0, baseHeight, 0});
        baseHeight+= 30;
        playerScoreDisplay->AddComponent<fovy::TextComponent>("Lorem Ipsum", smallerFont);
        const auto playerScoreDisplayComponent = playerScoreDisplay->AddComponent<fovy::ScoreDisplayComponent>();

        //Player
        const auto playerObj = std::make_shared<fovy::GameObject>("Player1");
        playerObj->GetTransform().SetWorldPosition(glm::vec3{100, 100 + (index * 100), 0});
        playerObj->GetTransform().SetLocalScale({0.2, 0.2, 0.2});
        playerObj->AddComponent<fovy::TextureComponent>(
            fovy::ResourceManager::GetInstance().LoadTexture(playerNames[index]));

        const auto playerScore = playerObj->AddComponent<fovy::ScoreComponent>();
        playerScore->GetScoreChangedEvent().AddListener(&fovy::AchievementManager::GetInstance(), &fovy::AchievementManager::ScoreChanged);
        playerScore->GetScoreChangedEvent().AddListener(playerScoreDisplayComponent,
                                                         &fovy::ScoreDisplayComponent::UpdateScore);
        playerScore->GetScoreChangedEvent().Invoke(0);

        const auto healthComponent = playerObj->AddComponent<fovy::HealthComponent>(3);


        auto livesDisplay = std::make_shared<fovy::GameObject>("Lives");
        livesDisplay->GetTransform().SetLocalPosition(glm::vec3{0, baseHeight, 0});
        baseHeight+= 40;

        livesDisplay->AddComponent<fovy::TextComponent>("hello", smallerFont);
        auto comp = livesDisplay->AddComponent<fovy::LivesDisplayComponent>();

        scene.Add(livesDisplay);
        scene.Add(playerObj);
        scene.Add(playerScoreDisplay);
        scene.Add(Player1Text);

        healthComponent->GetOnHealthChangeEvent().AddListener(comp, &fovy::LivesDisplayComponent::OnLivesUpdate);
        healthComponent->GetOnHealthChangeEvent().Invoke(3);

        constexpr float Player1MoveSpeed{200.f};

        auto& ToUse = index == 0 ? ControllerInputs : KeyboardInputs;
        for (int index1{0}; index1 < 4; ++index1) {
            fovy::InputManager::GetInstance().AddCommand<fovy::MoveCommand>(
                ToUse[index1], fovy::ButtonState::Down, playerObj.get(), Player1MoveSpeed,
                MoveDirections[index1]
            );
        }

        std::array<InputAction, 2> DamageButtons = {
            InputAction{{}, {XINPUT_GAMEPAD_X}},
            InputAction{{SDL_SCANCODE_C}, {}}
        };

        fovy::InputManager::GetInstance().AddCommand<fovy::FunctionCommand>(
            DamageButtons[index], fovy::Pressed, 0, [playerObj]() {
                playerObj->GetComponent<fovy::HealthComponent>()->Damage(1);
            }
        );

        std::array<InputAction, 2> ScoreButtons = {
            InputAction{{}, {XINPUT_GAMEPAD_B}},
            InputAction{{SDL_SCANCODE_X}, {}}
        };

        fovy::InputManager::GetInstance().AddCommand<fovy::FunctionCommand>(
           ScoreButtons[index], fovy::Pressed, 0, [playerObj]() {
                playerObj->GetComponent<fovy::ScoreComponent>()->AddPoints(100);
            }
        );
    }
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
    fs::path data_location = "./Data/";
    if (!fs::exists(data_location))
        data_location = "../Data/";
#endif
    fovy::Minigin engine(data_location);
    engine.Run(load);
    return 0;
}
