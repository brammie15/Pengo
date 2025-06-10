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
#include <iostream>
#include <XInput.h>

#include "GameCommands.h"
#include "ServiceLocator.h"
#include "Audio/SDLSoundSystem.h"
#include "Components/FPSComponent.h"
#include "Components/GridComponent.h"
#include "Components/Pengo/PengoComponent.h"
#include "../Minigin/Components/SpriteRenderer.h"
#include "../Minigin/Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Components/SnoBee/SnoBeeComponent.h"
#include "Components/Tile/TileComponent.h"
#include "Input/InputAction.h"
#include "Input/InputBinding.h"
#include "Input/InputManager.h"
#include "ObjectModel/GameObject.h"
#include "UI/Button.h"
#include "UI/Canvas.h"

namespace fs = std::filesystem;

void AddButtonToCanvas(
    const std::shared_ptr<fovy::GameObject>& button,
    const std::shared_ptr<fovy::GameObject>& canvas,
    const glm::vec3& position) {

    glm::vec2 buttonSize = button->GetComponent<fovy::TextComponent>()->GetSize();
    // Adjust the position to center the button on the canvas
    button->GetTransform().SetLocalPosition(glm::vec3(position.x - buttonSize.x / 2, position.y - buttonSize.y / 2, position.z));
    button->GetTransform().SetParent(&canvas->GetTransform());
}

void load() {

    auto& mainMenuScene = fovy::SceneManager::GetInstance().CreateScene("MainMenu");
    fovy::ServiceLocator<fovy::ISoundSystem>::RegisterService(std::make_unique<fovy::SDLSoundSystem>());

    auto testObject = std::make_shared<fovy::GameObject>("TestObject");
    [[maybe_unused]] auto testText = testObject->AddComponent<fovy::TextComponent>("P U S H", fovy::ResourceManager::GetInstance().LoadFont("pengo-arcade.otf", 20));

    mainMenuScene.Add(testObject);

    auto MainCanvas = std::make_shared<fovy::GameObject>("MainCanvas");
    auto canvasComponent = MainCanvas->AddComponent<fovy::Canvas>();

    auto OnePlayerButton = std::make_shared<fovy::GameObject>("1PlayerButton");
    [[maybe_unused]] auto buttonComponent = OnePlayerButton->AddComponent<fovy::Button>("1P Mode", fovy::ResourceManager::GetInstance().LoadFont("pengo-arcade.otf", 20));

    AddButtonToCanvas(OnePlayerButton, MainCanvas, glm::vec3(260, 515, 0));

    auto TwoPlayerButton = std::make_shared<fovy::GameObject>("2PlayerButton");
    [[maybe_unused]] auto button2Component = TwoPlayerButton->AddComponent<fovy::Button>("2P Mode", fovy::ResourceManager::GetInstance().LoadFont("pengo-arcade.otf", 20));

    AddButtonToCanvas(TwoPlayerButton, MainCanvas, glm::vec3(260, 615, 0));

    auto VersusButton = std::make_shared<fovy::GameObject>("Versus Mode");
    [[maybe_unused]] auto button3Component = VersusButton->AddComponent<fovy::Button>("Versus Mode", fovy::ResourceManager::GetInstance().LoadFont("pengo-arcade.otf", 20));

    AddButtonToCanvas(VersusButton, MainCanvas, glm::vec3(220, 715, 0));


    using fovy::InputAction;
    const std::array KeyboardInputs{
        InputAction{{SDL_SCANCODE_W, SDL_SCANCODE_UP}, {SDL_CONTROLLER_BUTTON_DPAD_UP}},
        InputAction{{SDL_SCANCODE_A, SDL_SCANCODE_LEFT}, {{SDL_CONTROLLER_BUTTON_DPAD_LEFT}}},
        InputAction{{SDL_SCANCODE_S, SDL_SCANCODE_DOWN}, {{SDL_CONTROLLER_BUTTON_DPAD_DOWN}}},
        InputAction{{SDL_SCANCODE_D, SDL_SCANCODE_RIGHT}, {{SDL_CONTROLLER_BUTTON_DPAD_RIGHT}}},
    };

    const std::array Directions{
        fovy::Direction::Up,
        fovy::Direction::Left,
        fovy::Direction::Down,
        fovy::Direction::Right
    };


    mainMenuScene.Add(MainCanvas);
    mainMenuScene.Add(OnePlayerButton);
    mainMenuScene.Add(TwoPlayerButton);
    mainMenuScene.Add(VersusButton);

    for (int index{0}; index < KeyboardInputs.size(); ++index) {
        fovy::InputManager::GetInstance().AddCommand<fovy::FunctionCommand>(
            KeyboardInputs[index],
            fovy::ButtonState::Pressed,
            [canvasComponent, Directions, index]() {
                canvasComponent->Move(Directions[index]);
            }
        );
    }



    auto& scene = fovy::SceneManager::GetInstance().CreateScene("Game");
    fovy::ServiceLocator<fovy::ISoundSystem>::RegisterService(std::make_unique<fovy::SDLSoundSystem>());
    fovy::SceneManager::GetInstance().SwitchScene(1);

    glm::vec2 windowSize = {224.f * 3.f, 288.f * 3.f};

    auto font = fovy::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    auto smallerFont = fovy::ResourceManager::GetInstance().LoadFont("Lingua.otf", 22);

    // const auto go = std::make_shared<fovy::GameObject>("Background");
    // go->AddComponent<fovy::TextureComponent>(fovy::ResourceManager::GetInstance().LoadTexture("background.tga"));
    // scene.Add(go);

    const auto testBackground = std::make_shared<fovy::GameObject>("TestBackground");
    auto backgroundTexture = fovy::ResourceManager::GetInstance().LoadTexture("screenshot.png");
    testBackground->AddComponent<fovy::TextureComponent>(backgroundTexture);

    testBackground->GetTransform().SetLocalScale(glm::vec3(1, 0.950, 1));

    //Center the background
    glm::vec2 textureSize = backgroundTexture->GetSize();
    glm::vec2 bgOffset = {textureSize.x / 2.f, textureSize.y / 2.f};
    bgOffset.x = (windowSize.x / 2.f) - bgOffset.x;
    bgOffset.y = (windowSize.y / 2.f) - bgOffset.y;
    testBackground->GetTransform().SetLocalPosition({bgOffset.x, bgOffset.y, 0.f});

    scene.Add(testBackground);

    const auto fpsGameobject = std::make_shared<fovy::GameObject>("FpsText");
    const auto textComponent = fpsGameobject->AddComponent<fovy::TextComponent>("", font);
    const auto fpsComponent = fpsGameobject->AddComponent<fovy::FPSComponent>();
    fpsComponent->SetTargetTextComponent(textComponent);

    scene.Add(fpsGameobject);

    const auto mainGridObject = std::make_shared<fovy::GameObject>("MainGrid");

    float cellWidth = 46.f;
    float cellHeight = 46.f;
    //Center the grid
    int GridWidth = 13;
    int GridHeight = 15;

    glm::vec2 offset = {static_cast<float>(GridWidth) * cellWidth / 2.f, static_cast<float>(GridHeight) * cellHeight / 2.f};
    offset.x = (windowSize.x / 2.f) - offset.x;
    offset.y = (windowSize.y / 2.f) - offset.y;

    mainGridObject->GetTransform().SetLocalPosition({offset.x, offset.y, 0.f});
    auto gridComponent = mainGridObject->AddComponent<GridComponent>(glm::ivec2{GridWidth, GridHeight}, glm::vec2{cellWidth, cellHeight});

    std::cout << "Starting load" << std::endl;
    gridComponent->LoadLevel("level1.txt");
    std::cout << "Ending load" << std::endl;

    scene.Add(mainGridObject);

    auto Player1 = std::make_shared<fovy::GameObject>("Player1");


    auto playerSprite = Player1->AddComponent<fovy::SpriteRenderer>();
    playerSprite->SetTexture("playerSpritesheet.png");
    playerSprite->SetTileIndex(0);

    playerSprite->AddAnimation("idle_up", {4}, 0.5f);
    playerSprite->AddAnimation("idle_down", {8}, 0.5f);
    playerSprite->AddAnimation("idle_left", {3}, 0.5f);
    playerSprite->AddAnimation("idle_right", {7}, 0.5f);

    playerSprite->AddAnimation("walk_up", {4, 5}, 0.5f, true);
    playerSprite->AddAnimation("walk_down", {8, 9}, 0.5f, true);
    playerSprite->AddAnimation("walk_left", {2,3}, 0.5f, true);
    playerSprite->AddAnimation("walk_right", {7, 6}, 0.5f, true);

    playerSprite->PlayAnimation("idle_up");

    auto playerComponent = Player1->AddComponent<pengo::PengoComponent>(gridComponent);



    constexpr std::array directions{
        MoveDirection::Up,
        MoveDirection::Left,
        MoveDirection::Down,
        MoveDirection::Right
    };

    for (int index{ 0 }; index < KeyboardInputs.size(); ++index) {
        const auto& input = KeyboardInputs[index];
        auto dir = directions[index];

        fovy::InputManager::GetInstance().AddCommand<PlayerMoveCommand>(
            input,
            fovy::ButtonState::Down,
            playerComponent,
            dir // sets m_HeldDirection
        );

        fovy::InputManager::GetInstance().AddCommand<PlayerMoveCommand>(
            input,
            fovy::ButtonState::Released,
            playerComponent,
            MoveDirection::None // clears m_HeldDirection
        );
    }

    fovy::InputManager::GetInstance().AddCommand<PlayerPushCommand>(
        InputAction{{SDL_SCANCODE_SPACE}, {SDL_CONTROLLER_BUTTON_A}},
        fovy::ButtonState::Pressed,
        playerComponent
    );


    scene.Add(Player1);

    // auto tileObject = std::make_shared<fovy::GameObject>("Tile");
    // [[maybe_unused]] auto tileComponent = tileObject->AddComponent<TileComponent>(gridComponent, glm::ivec2{0, 2});
    // auto tileSprite = tileObject->AddComponent<fovy::SpriteRenderer>();
    // tileSprite->SetTexture("PengoBlocks.png");
    // tileSprite->SetTileIndex(0);

    // scene.Add(tileObject);

    auto testSnoBee = std::make_shared<fovy::GameObject>("TestSnoBee");
    auto snoBeeSprite = testSnoBee->AddComponent<fovy::SpriteRenderer>();
    snoBeeSprite->SetTexture("PengoSnoBee.png");
    snoBeeSprite->SetTileIndex(0);
    snoBeeSprite->AddAnimation("spawn", {0, 1, 2, 3, 4, 5}, 0.2f, false);
    snoBeeSprite->AddAnimation("idle_down", {9}, 0.5f, true);
    snoBeeSprite->AddAnimation("idle_left", {11}, 0.5f, true);
    snoBeeSprite->AddAnimation("idle_up", {13}, 0.5f, true);
    snoBeeSprite->AddAnimation("idle_right", {15}, 0.5f, true);

    snoBeeSprite->AddAnimation("walk_down", {8, 9}, 0.5f, true);
    snoBeeSprite->AddAnimation("walk_left", {10, 11}, 0.5f, true);
    snoBeeSprite->AddAnimation("walk_up", {12,13}, 0.5f, true);
    snoBeeSprite->AddAnimation("walk_right", {14,15}, 0.5f, true);


    auto snoBeeComponent = testSnoBee->AddComponent<pengo::SnoBeeComponent>(gridComponent, glm::vec2{0.f, 2.f});

    const std::array testInputs{
        InputAction{{SDL_SCANCODE_I, SDL_SCANCODE_UP}, {}},
        InputAction{{SDL_SCANCODE_J, SDL_SCANCODE_LEFT}, {{}}},
        InputAction{{SDL_SCANCODE_K, SDL_SCANCODE_DOWN}, {{}}},
        InputAction{{SDL_SCANCODE_L, SDL_SCANCODE_RIGHT}, {{}}},
    };

    for (int index{ 0 }; index < KeyboardInputs.size(); ++index) {
        const auto& input = testInputs[index];
        auto dir = directions[index];

        fovy::InputManager::GetInstance().AddCommand<SnoBeeMoveCommand>(
            input,
            fovy::ButtonState::Down,
            snoBeeComponent,
            dir // sets m_HeldDirection
        );

        fovy::InputManager::GetInstance().AddCommand<SnoBeeMoveCommand>(
            input,
            fovy::ButtonState::Released,
            snoBeeComponent,
            MoveDirection::None // clears m_HeldDirection
        );
    }

    scene.Add(testSnoBee);
    fovy::SceneManager::GetInstance().SwitchScene(0);




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
