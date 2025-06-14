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
#include "GameController.h"
#include "Prefabs.h"
#include "ServiceLocator.h"
#include "../Minigin/Components/SpriteRenderer.h"
#include "../Minigin/Components/TextComponent.h"
#include "Audio/SDLSoundSystem.h"
#include "Components/EggDisplay.h"
#include "Components/FPSComponent.h"
#include "Components/GridComponent.h"
#include "Components/LivesDisplayComponent.h"
#include "Components/MainGameController.h"
#include "Components/ScoreScreenManager.h"
#include "Components/TextureComponent.h"
#include "Components/Pengo/PengoComponent.h"
#include "Components/SnoBee/SnoBeeComponent.h"
#include "Components/SnoBee/SnoBeeController.h"
#include "Components/Tile/TileComponent.h"
#include "Components/UI/LetterRoller.h"
#include "Components/UI/MainMenuUiManager.h"
#include "Input/InputAction.h"
#include "Input/InputBinding.h"
#include "Input/InputManager.h"
#include "ObjectModel/GameObject.h"
#include "UI/Button.h"
#include "UI/Canvas.h"

namespace fs = std::filesystem;

const std::array KeyboardInputs{
    fovy::InputAction{{SDL_SCANCODE_W, SDL_SCANCODE_UP}, {SDL_CONTROLLER_BUTTON_DPAD_UP}},
    fovy::InputAction{{SDL_SCANCODE_A, SDL_SCANCODE_LEFT}, {{SDL_CONTROLLER_BUTTON_DPAD_LEFT}}},
    fovy::InputAction{{SDL_SCANCODE_S, SDL_SCANCODE_DOWN}, {{SDL_CONTROLLER_BUTTON_DPAD_DOWN}}},
    fovy::InputAction{{SDL_SCANCODE_D, SDL_SCANCODE_RIGHT}, {{SDL_CONTROLLER_BUTTON_DPAD_RIGHT}}},
};

constexpr std::array Directions{
    fovy::Direction::Up,
    fovy::Direction::Left,
    fovy::Direction::Down,
    fovy::Direction::Right
};


void load() {
    constexpr auto windowSize = glm::vec2(224.f * 3.f, 288.f * 3.f); //TODO: make this global

    auto& mainMenuScene = fovy::SceneManager::GetInstance().CreateScene("MainMenu");
    fovy::ServiceLocator<fovy::ISoundSystem>::RegisterService(std::make_unique<fovy::SDLSoundSystem>());

    auto pengoLogo = std::make_shared<fovy::GameObject>("PengoLogo");
    pengoLogo->AddComponent<fovy::TextureComponent>(fovy::ResourceManager::GetInstance().LoadTexture("Logo.png"));
    pengoLogo->GetTransform().SetWorldPosition(glm::vec3(120, 100, 0));
    pengoLogo->GetTransform().SetLocalScale(3, 3, 1);


    mainMenuScene.Add(pengoLogo);

    auto copyrightText = std::make_shared<fovy::GameObject>("CopyrightText");
    copyrightText->AddComponent<fovy::TextComponent>("Bram Verhulst - 2025", fovy::ResourceManager::GetInstance().LoadFont("pengo-arcade.otf", 20));
    copyrightText->GetTransform().SetWorldPosition(glm::vec3(150, windowSize.y - 50, 0));

    mainMenuScene.Add(copyrightText);

    auto MainCanvas = std::make_shared<fovy::GameObject>("MainCanvas");
    auto canvasComponent = MainCanvas->AddComponent<fovy::Canvas>();

    canvasComponent->GetOnFocusChanged().AddListener([canvasComponent] (fovy::GameObject*) {
        fovy::ServiceLocator<fovy::ISoundSystem>::GetService().PlayAsync("Data/swipe.wav", 0.2f, 0);
    });

    auto OnePlayerButton = std::make_shared<fovy::GameObject>("1PlayerButton");
    auto buttonComponent = OnePlayerButton->AddComponent<fovy::Button>("1P Mode", fovy::ResourceManager::GetInstance().LoadFont("pengo-arcade.otf", 20));


    auto MMMObject = std::make_shared<fovy::GameObject>("MainMenuManager");
    auto mainUiManager = MMMObject->AddComponent<pengo::MainMenuUiManager>();


    buttonComponent->GetClickEvent().AddListener([mainUiManager, &mainMenuScene]() {
        pengo::GameController::GetInstance().SetGameMode(pengo::GameMode::Singleplayer);
        mainUiManager->OnStartGame();
        fovy::ServiceLocator<fovy::ISoundSystem>::GetService().PlayAsync("Data/click.wav", 0.2f, 0);
    });


    mainMenuScene.Add(MMMObject);

    pengo::AddButtonToCanvas(OnePlayerButton, MainCanvas, glm::vec3(265, 515, 0));

    auto TwoPlayerButton = std::make_shared<fovy::GameObject>("2PlayerButton");
    [[maybe_unused]] auto button2Component = TwoPlayerButton->AddComponent<fovy::Button>("2P Mode", fovy::ResourceManager::GetInstance().LoadFont("pengo-arcade.otf", 20));

    pengo::AddButtonToCanvas(TwoPlayerButton, MainCanvas, glm::vec3(265, 615, 0));

    button2Component->GetClickEvent().AddListener([mainUiManager]() {
        pengo::GameController::GetInstance().SetGameMode(pengo::GameMode::Multiplayer);
        mainUiManager->OnStartGame();
    });

    auto VersusButton = std::make_shared<fovy::GameObject>("Versus Mode");
    [[maybe_unused]] auto button3Component = VersusButton->AddComponent<fovy::Button>("Versus Mode", fovy::ResourceManager::GetInstance().LoadFont("pengo-arcade.otf", 20));

    pengo::AddButtonToCanvas(VersusButton, MainCanvas, glm::vec3(227, 715, 0));

    button3Component->GetClickEvent().AddListener([mainUiManager]() {
        pengo::GameController::GetInstance().SetGameMode(pengo::GameMode::Versus);
        mainUiManager->OnStartGame();
    });


    auto DancingPengo = std::make_shared<fovy::GameObject>("DancingPengo");
    auto pengoComponent = DancingPengo->AddComponent<fovy::SpriteRenderer>();
    pengoComponent->SetTexture(fovy::ResourceManager::GetInstance().LoadTexture("playerSpritesheet.png"));

    pengoComponent->AddAnimation("dance", {20, 21, 20, 21, 20, 21, 20, 21, 22, 23, 22, 23, 22, 23, 22, 23}, 0.1f, true);
    pengoComponent->PlayAnimation("dance");

    DancingPengo->GetTransform().SetWorldPosition(glm::vec3(200, 400, 0));
    DancingPengo->GetTransform().SetWorldScale(glm::vec3(1.5f, 1.5f, 1));


    mainMenuScene.Add(DancingPengo);


    using fovy::InputAction;

    mainMenuScene.SetRegisterBindings([canvasComponent]() {
        for (int index{0}; index < KeyboardInputs.size(); ++index) {
            fovy::InputManager::GetInstance().AddCommand<fovy::FunctionCommand>(
                KeyboardInputs[index],
                fovy::ButtonState::Pressed,
                [canvasComponent, index]() {
                    canvasComponent->Move(Directions[index]);
                }
            );
        }

        fovy::InputManager::GetInstance().AddCommand<fovy::FunctionCommand>(
            InputAction{{SDL_SCANCODE_SPACE}, {SDL_CONTROLLER_BUTTON_A}},
            fovy::ButtonState::Pressed,
            [canvasComponent]() {
                canvasComponent->Interact();
            }
        );
    });

    mainMenuScene.SetUnregisterBindings([]() {
        fovy::InputManager::GetInstance().RemoveBindings(InputAction{{SDL_SCANCODE_W, SDL_SCANCODE_UP}, {SDL_CONTROLLER_BUTTON_DPAD_UP}});
        fovy::InputManager::GetInstance().RemoveBindings(InputAction{{SDL_SCANCODE_A, SDL_SCANCODE_LEFT}, {{SDL_CONTROLLER_BUTTON_DPAD_LEFT}}});
        fovy::InputManager::GetInstance().RemoveBindings(InputAction{{SDL_SCANCODE_S, SDL_SCANCODE_DOWN}, {{SDL_CONTROLLER_BUTTON_DPAD_DOWN}}});
        fovy::InputManager::GetInstance().RemoveBindings(InputAction{{SDL_SCANCODE_D, SDL_SCANCODE_RIGHT}, {{SDL_CONTROLLER_BUTTON_DPAD_RIGHT}}});
        fovy::InputManager::GetInstance().RemoveBindings(InputAction{{SDL_SCANCODE_SPACE}, {SDL_CONTROLLER_BUTTON_A}});
    });


    mainMenuScene.Add(MainCanvas);
    mainMenuScene.Add(OnePlayerButton);
    mainMenuScene.Add(TwoPlayerButton);
    mainMenuScene.Add(VersusButton);

    for (int index{0}; index < KeyboardInputs.size(); ++index) {
        fovy::InputManager::GetInstance().AddCommand<fovy::FunctionCommand>(
            KeyboardInputs[index],
            fovy::ButtonState::Pressed,
            [canvasComponent, index]() {
                canvasComponent->Move(Directions[index]);
            }
        );
    }

    fovy::InputManager::GetInstance().AddCommand<fovy::FunctionCommand>(
        InputAction{{SDL_SCANCODE_SPACE}, {SDL_CONTROLLER_BUTTON_A}},
        fovy::ButtonState::Pressed,
        [canvasComponent]() {
            canvasComponent->Interact();
        }
    );


    auto& scene = fovy::SceneManager::GetInstance().CreateScene("Game");
    fovy::ServiceLocator<fovy::ISoundSystem>::RegisterService(std::make_unique<fovy::SDLSoundSystem>());
    fovy::SceneManager::GetInstance().SwitchScene(1);


    auto font = fovy::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    auto smallerFont = fovy::ResourceManager::GetInstance().LoadFont("Lingua.otf", 22);

    // const auto go = std::make_shared<fovy::GameObject>("Background");
    // go->AddComponent<fovy::TextureComponent>(fovy::ResourceManager::GetInstance().LoadTexture("background.tga"));
    // scene.Add(go);

    // const auto testBackground = std::make_shared<fovy::GameObject>("TestBackground");
    // auto backgroundTexture = fovy::ResourceManager::GetInstance().LoadTexture("screenshot.png");
    // testBackground->AddComponent<fovy::TextureComponent>(backgroundTexture);
    //
    // testBackground->GetTransform().SetLocalScale(glm::vec3(1, 0.950, 1));
    //
    // //Center the background
    // glm::vec2 textureSize = backgroundTexture->GetSize();
    // glm::vec2 bgOffset = {textureSize.x / 2.f, textureSize.y / 2.f};
    // bgOffset.x = (windowSize.x / 2.f) - bgOffset.x;
    // bgOffset.y = (windowSize.y / 2.f) - bgOffset.y;
    // testBackground->GetTransform().SetLocalPosition({bgOffset.x, bgOffset.y, 0.f});
    //
    // scene.Add(testBackground);

    // const auto fpsGameobject = std::make_shared<fovy::GameObject>("FpsText");
    // const auto textComponent = fpsGameobject->AddComponent<fovy::TextComponent>("", font);
    // const auto fpsComponent = fpsGameobject->AddComponent<fovy::FPSComponent>();
    // fpsComponent->SetTargetTextComponent(textComponent);
    //
    // scene.Add(fpsGameobject);

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

    playerSprite->AddAnimation("walk_up", {4, 5, 4, 5}, 1.f, true);
    playerSprite->AddAnimation("walk_down", {8, 9, 8, 9}, 1.f, true);
    playerSprite->AddAnimation("walk_left", {2, 3, 2, 3}, 1.0f, true);
    playerSprite->AddAnimation("walk_right", {7, 6, 7, 6}, 1.0f, true);

    playerSprite->AddAnimation("push_left", {10, 11}, 0.1f, false);
    playerSprite->AddAnimation("push_right", {14, 15}, 0.1f, true);
    playerSprite->AddAnimation("push_up", {4, 5}, 0.1f, true);
    playerSprite->AddAnimation("push_down", {8, 9}, 0.1f, true);

    playerSprite->AddAnimation("die", {16, 17, 16, 17, 16, 17}, 0.1f, false);

    playerSprite->AddAnimation("die", {16, 17, 16, 17, 16, 17}, 0.1f, false);

    playerSprite->PlayAnimation("idle_up");

    glm::ivec2 spawnPos = gridComponent->GetPlayerSpawns()[0];
    Player1->GetTransform().SetWorldPosition(gridComponent->WorldPositionFromGrid(spawnPos));

    auto playerComponent = Player1->AddComponent<pengo::PengoComponent>(gridComponent);


    scene.Add(Player1);

    auto wallShake = fovy::Animation{
        {0, 1, 0, 1, 0, 1, 0, 1, 2},
        0.1f,
        false
    };


    auto topWallObject = std::make_shared<fovy::GameObject>("TopWall");
    auto topWallComponent = topWallObject->AddComponent<fovy::SpriteRenderer>();
    topWallComponent->SetTexture("wall-horizontal.png");
    topWallComponent->SetTileSize(224, 8);
    topWallComponent->SetDestTileSize(214 * 3, 6 * 3 + 2);
    topWallComponent->SetTileIndex(2);
    topWallComponent->AddAnimation("shake", wallShake.frames, wallShake.frameDuration, wallShake.loop);


    topWallObject->GetTransform().SetWorldPosition(glm::vec3(16, 70, 0));

    scene.Add(topWallObject);

    auto bottomWallObject = std::make_shared<fovy::GameObject>("BottomWall");
    auto bottomWallComponent = bottomWallObject->AddComponent<fovy::SpriteRenderer>();
    bottomWallComponent->SetTexture("wall-horizontal.png");
    bottomWallComponent->SetTileSize(224, 8);
    bottomWallComponent->SetDestTileSize(214 * 3, 6 * 3 + 2);
    bottomWallComponent->SetTileIndex(2);
    bottomWallComponent->AddAnimation("shake", wallShake.frames, wallShake.frameDuration, wallShake.loop);


    bottomWallObject->GetTransform().SetWorldPosition(glm::vec3(16, 777, 0));

    scene.Add(bottomWallObject);

    auto leftWallObject = std::make_shared<fovy::GameObject>("LeftWall");
    auto leftWallComponent = leftWallObject->AddComponent<fovy::SpriteRenderer>();
    leftWallComponent->SetTexture("wall-vertical.png");
    leftWallComponent->SetTileSize(8, 224);
    leftWallComponent->SetDestTileSize(6 * 3 + 2, 240 * 3);
    leftWallComponent->SetTileIndex(2);
    leftWallComponent->AddAnimation("shake", wallShake.frames, wallShake.frameDuration, wallShake.loop);

    leftWallObject->GetTransform().SetWorldPosition(glm::vec3(14, 70, 0));

    scene.Add(leftWallObject);

    auto rightWallObject = std::make_shared<fovy::GameObject>("RightWall");
    auto rightWallComponent = rightWallObject->AddComponent<fovy::SpriteRenderer>();
    rightWallComponent->SetTexture("wall-vertical.png");
    rightWallComponent->SetTileSize(8, 224);
    rightWallComponent->SetDestTileSize(6 * 3 + 2, 240 * 3);
    rightWallComponent->SetTileIndex(2);
    rightWallComponent->AddAnimation("shake", wallShake.frames, wallShake.frameDuration, wallShake.loop);

    rightWallObject->GetTransform().SetWorldPosition(glm::vec3(640, 70, 0));

    scene.Add(rightWallObject);

    std::map<fovy::Direction, fovy::GameObject*> wallObjects{
        {fovy::Direction::Up, topWallObject.get()},
        {fovy::Direction::Down, bottomWallObject.get()},
        {fovy::Direction::Left, leftWallObject.get()},
        {fovy::Direction::Right, rightWallObject.get()}
    };

    playerComponent->GetWallPushEvent().AddListener([topWallObject, gridComponent] (fovy::Direction dir) {
        if (dir != fovy::Direction::Up) return;
        topWallObject->GetComponent<fovy::SpriteRenderer>()->PlayAnimation("shake", true);
        fovy::ServiceLocator<fovy::ISoundSystem>::GetService().PlayAsync("Data/Sounds/push_wall.wav", 0.2f, 0);

        for (int x{0}; x < 13; ++x) {
            fovy::GameObject* occupant = gridComponent->GetCell({x, 0}).occupant;
            if (occupant && occupant->HasComponent<pengo::SnoBeeComponent>()) {
                auto snoBeeComponent = occupant->GetComponent<pengo::SnoBeeComponent>();
                snoBeeComponent->Stun();
            }
        }
    });

    playerComponent->GetWallPushEvent().AddListener([bottomWallObject, gridComponent] (fovy::Direction dir) {
        if (dir != fovy::Direction::Down) return;
        bottomWallObject->GetComponent<fovy::SpriteRenderer>()->PlayAnimation("shake", true);
        fovy::ServiceLocator<fovy::ISoundSystem>::GetService().PlayAsync("Data/Sounds/push_wall.wav", 0.2f, 0);
        for (int x{0}; x < 13; ++x) {
            int height = gridComponent->GetHeight() - 1;
            fovy::GameObject* occupant = gridComponent->GetCell({x, height}).occupant;
            if (occupant && occupant->HasComponent<pengo::SnoBeeComponent>()) {
                auto snoBeeComponent = occupant->GetComponent<pengo::SnoBeeComponent>();
                snoBeeComponent->Stun();
            }
        }
    });

    playerComponent->GetWallPushEvent().AddListener([leftWallObject, gridComponent] (fovy::Direction dir) {
        if (dir != fovy::Direction::Left) return;
        leftWallObject->GetComponent<fovy::SpriteRenderer>()->PlayAnimation("shake", true);
        fovy::ServiceLocator<fovy::ISoundSystem>::GetService().PlayAsync("Data/Sounds/push_wall.wav", 0.2f, 0);

        for (int Y{0}; Y < 15; ++Y) {
            fovy::GameObject* occupant = gridComponent->GetCell({0, Y}).occupant;
            if (occupant && occupant->HasComponent<pengo::SnoBeeComponent>()) {
                auto snoBeeComponent = occupant->GetComponent<pengo::SnoBeeComponent>();
                snoBeeComponent->Stun();
            }
        }
    });

    playerComponent->GetWallPushEvent().AddListener([rightWallObject, gridComponent] (fovy::Direction dir) {
        if (dir != fovy::Direction::Right) return;
        rightWallObject->GetComponent<fovy::SpriteRenderer>()->PlayAnimation("shake", true);
        fovy::ServiceLocator<fovy::ISoundSystem>::GetService().PlayAsync("Data/Sounds/push_wall.wav", 0.2f, 0);

        for (int Y{0}; Y < 15; ++Y) {
            int width = gridComponent->GetWidth() - 1;
            fovy::GameObject* occupant = gridComponent->GetCell({width, Y}).occupant;
            if (occupant && occupant->HasComponent<pengo::SnoBeeComponent>()) {
                auto snoBeeComponent = occupant->GetComponent<pengo::SnoBeeComponent>();
                snoBeeComponent->Stun();
            }
        }
    });

    // auto tileObject = std::make_shared<fovy::GameObject>("Tile");
    // [[maybe_unused]] auto tileComponent = tileObject->AddComponent<TileComponent>(gridComponent, glm::ivec2{0, 2});
    // auto tileSprite = tileObject->AddComponent<fovy::SpriteRenderer>();
    // tileSprite->SetTexture("PengoBlocks.png");
    // tileSprite->SetTileIndex(0);

    // scene.Add(tileObject);
    //
    // auto testSnoBee = std::make_shared<fovy::GameObject>("TestSnoBee");
    // auto snoBeeSprite = testSnoBee->AddComponent<fovy::SpriteRenderer>();
    // snoBeeSprite->SetTexture("PengoSnoBee.png");
    // snoBeeSprite->SetTileIndex(0);
    // snoBeeSprite->AddAnimation("spawn", {0, 1, 2, 3, 4, 5}, 0.2f, false);
    // snoBeeSprite->AddAnimation("idle_down", {9}, 0.5f, true);
    // snoBeeSprite->AddAnimation("idle_left", {11}, 0.5f, true);
    // snoBeeSprite->AddAnimation("idle_up", {13}, 0.5f, true);
    // snoBeeSprite->AddAnimation("idle_right", {15}, 0.5f, true);
    //
    // snoBeeSprite->AddAnimation("walk_down", {8, 9}, 0.5f, true);
    // snoBeeSprite->AddAnimation("walk_left", {10, 11}, 0.5f, true);
    // snoBeeSprite->AddAnimation("walk_up", {12, 13}, 0.5f, true);
    // snoBeeSprite->AddAnimation("walk_right", {14, 15}, 0.5f, true);
    //
    //
    // auto snoBeeComponent = testSnoBee->AddComponent<pengo::SnoBeeComponent>(gridComponent, glm::vec2{0.f, 2.f});
    //
    // auto controller = testSnoBee->AddComponent<pengo::SnoBeeController>(gridComponent);
    // controller->SetTarget(Player1.get());
    // scene.Add(testSnoBee);

    std::vector<glm::ivec2> randomEnemyPositions(3);
    //3 random positions in the grid
    for (int i = 0; i < randomEnemyPositions.size(); ++i) {
        bool foundPosition = false;
        glm::ivec2 randomPos{};
        while (!foundPosition) {
            randomPos = glm::ivec2{
                rand() % gridComponent->GetWidth(),
                rand() % gridComponent->GetHeight()
            };
            if (!gridComponent->IsOccupied(randomPos)) {
                randomEnemyPositions[i] = randomPos;
                foundPosition = true;
            }
        }
        randomEnemyPositions[i] = randomPos;
    }

    for (auto pos: randomEnemyPositions) {
        pengo::AddEnemy(&scene, gridComponent, pos, "SnoBee1");
    }


    auto scoreFont = fovy::ResourceManager::GetInstance().LoadFont("pengo-arcade.otf", 20);

    auto ScoreDisplay = std::make_shared<fovy::GameObject>("ScoreDisplay");
    auto scoreTextComponent = ScoreDisplay->AddComponent<fovy::TextComponent>("1P 0", scoreFont);
    scoreTextComponent->SetColor(SDL_Color(0, 255, 255, 255));
    ScoreDisplay->GetTransform().SetLocalPosition(glm::vec3(40, 4, 0));

    pengo::GameController::GetInstance().GetOnScoreChangedEvent().AddListener([scoreTextComponent] (int score) {
        scoreTextComponent->SetText("1P " + std::to_string(score));
    });

    scene.Add(ScoreDisplay);

    auto eggCounter = std::make_shared<fovy::GameObject>("EggCounter");
    auto eggCounterComponent = eggCounter->AddComponent<pengo::EggDisplay>();

    eggCounter->GetTransform().SetWorldPosition(glm::vec3(280, 50, 0));

    scene.Add(eggCounter);

    auto livesCounter = std::make_shared<fovy::GameObject>("LivesCounter");
    auto livesDisplay = livesCounter->AddComponent<fovy::LivesDisplayComponent>();
    livesCounter->GetTransform().SetWorldPosition(glm::vec3(14, 31, 0));


    scene.Add(livesCounter);


    scene.SetRegisterBindings([playerComponent]() {
    });
    auto mainGameManager = std::make_shared<fovy::GameObject>("MainGameManager");
    auto gameController = mainGameManager->AddComponent<pengo::MainGameController>(playerComponent, gridComponent, nullptr, wallObjects);

    pengo::GameController::GetInstance().SetMainGameController(gameController);

    gameController->GetOnLivesChangeEvent().AddListener(livesDisplay, &fovy::LivesDisplayComponent::OnLivesUpdate);

    scene.Add(mainGameManager);

    for (auto infested_tile: gridComponent->GetInfestedTiles()) {
        gameController->AddInftestedtile(infested_tile);
    }

    gameController->GetOnInfestedTilesChange().AddListener([eggCounterComponent] (int eggCount) {
        eggCounterComponent->UpdateEggCount(eggCount);
    });


    scene.OnSceneLoaded.AddListener([gameController, gridComponent]() {
        gameController->SetupBindings(gridComponent);
        fovy::ServiceLocator<fovy::ISoundSystem>::GetService().PlayAsync("Data/Sounds/main_bgm.mp3", 0.1f, 100);
    }); {
        auto& scoreScene = fovy::SceneManager::GetInstance().CreateScene("Score");

        auto scoreCanvas = std::make_shared<fovy::GameObject>("ScoreCanvas");
        auto scoreCanvasComponent = scoreCanvas->AddComponent<fovy::Canvas>();
        scoreScene.Add(scoreCanvas);

        //18, 30

        auto letterRollerFont = fovy::ResourceManager::GetInstance().LoadFont("pengo-arcade.otf", 40);
        auto enterNameFont = fovy::ResourceManager::GetInstance().LoadFont("pengo-arcade.otf", 20);

        auto EnterNameObject = std::make_shared<fovy::GameObject>("EnterNameText");
        EnterNameObject->AddComponent<fovy::TextComponent>("Enter Name.", enterNameFont);
        EnterNameObject->GetTransform().SetLocalPosition(glm::vec3(50, 470, 0));

        scoreScene.Add(EnterNameObject);

        auto highScoreHeader = std::make_shared<fovy::GameObject>("HighScoreHeader");
        highScoreHeader->AddComponent<fovy::TextComponent>("High Scores", enterNameFont);
        highScoreHeader->GetTransform().SetLocalPosition(glm::vec3(50, 38, 0));

        scoreScene.Add(highScoreHeader);


        auto* roller1 = pengo::CreateLetterRoller(&scoreScene, scoreCanvas.get(), glm::vec3(200, 500, 0), letterRollerFont, "Letter1");
        auto* roller2 = pengo::CreateLetterRoller(&scoreScene, scoreCanvas.get(), glm::vec3(300, 500, 0), letterRollerFont, "Letter2");
        auto* roller3 = pengo::CreateLetterRoller(&scoreScene, scoreCanvas.get(), glm::vec3(400, 500, 0), letterRollerFont, "Letter3");

        auto scoreScreenManager = std::make_shared<fovy::GameObject>("ScoreScreenManager");
        auto scoreCanvasManager = scoreScreenManager->AddComponent<pengo::ScoreScreenManager>(std::array{
                                                                                                  roller1->GetComponent<pengo::LetterRoller>(),
                                                                                                  roller2->GetComponent<pengo::LetterRoller>(),
                                                                                                  roller3->GetComponent<pengo::LetterRoller>()
                                                                                              }, scoreCanvasComponent);

        auto scoreButton = std::make_shared<fovy::GameObject>("ScoreButton");
        auto scoreButtonComponent = scoreButton->AddComponent<fovy::Button>("Submit Score", fovy::ResourceManager::GetInstance().LoadFont("pengo-arcade.otf", 20));
        scoreButton->GetTransform().SetLocalPosition(glm::vec3(225, 700, 0));
        scoreButtonComponent->GetClickEvent().AddListener([scoreCanvasManager, canvasComponent]() {
            scoreCanvasManager->SubmitScore(canvasComponent);
        });
        scoreButton->GetTransform().SetParent(&scoreCanvas->GetTransform());

        roller3->GetComponent<pengo::LetterRoller>()->SetNavigationDirection(
            fovy::Direction::Right,
            scoreButton.get()
        );

        buttonComponent->SetNavigationDirection(
            fovy::Direction::Left,
            roller3->GetTransform().GetOwner()
        );

        buttonComponent->SetNavigationDirection(
            fovy::Direction::Right,
            roller3->GetTransform().GetOwner()
        );

        // buttonComponent->SetNavigationDirection(
        //     fovy::Direction::Up,
        //     nullptr
        // );

        scoreScene.Add(scoreScreenManager);
        scoreScene.Add(scoreButton);


        scoreScene.SetRegisterBindings([scoreCanvasComponent]() {
            for (int index{0}; index < KeyboardInputs.size(); ++index) {
                fovy::InputManager::GetInstance().AddCommand<fovy::FunctionCommand>(
                    KeyboardInputs[index],
                    fovy::ButtonState::Pressed,
                    [scoreCanvasComponent, index]() {
                        scoreCanvasComponent->Move(Directions[index]);
                    }
                );
            }

            fovy::InputManager::GetInstance().AddCommand<fovy::FunctionCommand>(
                InputAction{{SDL_SCANCODE_SPACE}, {SDL_CONTROLLER_BUTTON_A}},
                fovy::ButtonState::Pressed,
                [scoreCanvasComponent]() {
                    scoreCanvasComponent->Interact();
                }
            );
        });
    }
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
