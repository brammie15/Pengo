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

#include "Components/FPSComponent.h"
#include "Components/HealthComponent.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Components/ThrashTheCacheComponent.h"
#include "Input/InputAction.h"
#include "Input/InputBinding.h"
#include "Input/InputManager.h"
#include "ObjectModel/GameObject.h"

namespace fs = std::filesystem;

void load() {
    auto& scene = fovy::SceneManager::GetInstance().CreateScene("Demo");

    const auto go = std::make_shared<fovy::GameObject>("Background");
    go->AddComponent<fovy::TextureComponent>(fovy::ResourceManager::GetInstance().LoadTexture("background.tga"));
    scene.Add(go);

    auto font = fovy::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

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
    //
    // const auto parentObj = std::make_shared<dae::GameObject>("ParentObj");
    // const auto childObj = std::make_shared<dae::GameObject>("ChildObj");
    //
    // const auto childImage = dae::ResourceManager::GetInstance().LoadTexture("child.jpg");
    // const auto parentImage = dae::ResourceManager::GetInstance().LoadTexture("Parent.jpg");
    //
    // parentObj->AddComponent<dae::TextureComponent>(parentImage);
    // parentObj->GetTransform().SetWorldPosition(glm::vec3{200, 200, 0});
    // parentObj->AddComponent<dae::Rotator>(30.f, 10.f);
    // // parentObj->GetTransform().AddChild(&childObj->GetTransform());
    //
    // childObj->GetTransform().SetParent(&parentObj->GetTransform());
    // childObj->AddComponent<dae::TextureComponent>(childImage);
    // childObj->AddComponent<dae::Rotator>(30.f, 5.f);
    // childObj->GetTransform().SetLocalPosition(glm::vec3{50, 50, 0});
    //
    // scene.Add(parentObj);
    // scene.Add(childObj);


    auto player1Obj = std::make_shared<fovy::GameObject>("Player1");
    player1Obj->AddComponent<fovy::TextureComponent>(fovy::ResourceManager::GetInstance().LoadTexture("Player1.png"));
    player1Obj->GetTransform().SetWorldPosition(glm::vec3{100, 100, 0});
    auto healthComponent = player1Obj->AddComponent<fovy::HealthComponent>(3);

    healthComponent->GetOnHealthChangeEvent().AddListener([](int newAmount) {
        std::cout << newAmount << std::endl;
    });

    // fovy::InputManager::GetInstance().AddCommand<>()

    scene.Add(player1Obj);

    auto player2Obj = std::make_shared<fovy::GameObject>("Player2");
    player2Obj->AddComponent<fovy::TextureComponent>(fovy::ResourceManager::GetInstance().LoadTexture("Player2.png"));
    player2Obj->GetTransform().SetWorldPosition(glm::vec3{300, 100, 0});

    scene.Add(player2Obj);


    constexpr float Player1MoveSpeed{ 200.f };
    constexpr float Player2MoveSpeed{ Player1MoveSpeed * 2 };

    using fovy::InputAction;

    std::array<InputAction, 4> KeyboardInputs{
        InputAction{{SDL_SCANCODE_W}},
        InputAction{{SDL_SCANCODE_A}},
        InputAction{{SDL_SCANCODE_S}},
        InputAction{{SDL_SCANCODE_D}},
    };

    std::array<InputAction, 4> ControllerInputs{
        InputAction{{},{XINPUT_GAMEPAD_DPAD_UP}},
        InputAction{{},{XINPUT_GAMEPAD_DPAD_LEFT}},
        InputAction{{},{XINPUT_GAMEPAD_DPAD_DOWN}},
        InputAction{{},{XINPUT_GAMEPAD_DPAD_RIGHT}},
    };

    std::array<glm::vec3, 4> MoveDirections{
        glm::vec3{0, -1, 0},
        glm::vec3{-1, 0, 0},
        glm::vec3{0, 1, 0},
        glm::vec3{1, 0, 0},
    };

    for (int index{0}; index < 4; ++index) {
        fovy::InputManager::GetInstance().AddCommand<fovy::MoveCommand>(
            ControllerInputs[index], fovy::ButtonState::Down, player1Obj.get(), Player1MoveSpeed, MoveDirections[index]
        );

        fovy::InputManager::GetInstance().AddCommand<fovy::MoveCommand>(
          ControllerInputs[index], fovy::ButtonState::Down, 1, player2Obj.get(), Player2MoveSpeed, MoveDirections[index]
      );
    }

    fovy::InputManager::GetInstance().AddCommand<fovy::ConsoleLogCommand>(
        InputAction{{SDL_SCANCODE_T}, {XINPUT_GAMEPAD_Y}}, fovy::Pressed, 0, "T or Y presssed"
    );

    fovy::InputManager::GetInstance().AddCommand<fovy::FunctionCommand>(
        InputAction{{},{XINPUT_GAMEPAD_A}}, fovy::Pressed, 0, [player1Obj]() {
            player1Obj->GetComponent<fovy::HealthComponent>()->Damage(1);
        }
    );


    // auto ThrashTheCacheObj = std::make_shared<dae::GameObject>("ThrashTheCache");
    // ThrashTheCacheObj->AddComponent<dae::ThrashTheCacheComponent>();
    //
    // scene.Add(ThrashTheCacheObj);
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
