#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "Managers/ResourceManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#include <filesystem>

#include "Components/FPSComponent.h"
#include "Components/Rotator.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Components/ThrashTheCacheComponent.h"
#include "ObjectModel/GameObject.h"

namespace fs = std::filesystem;

void load() {
    auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

    const auto go = std::make_shared<dae::GameObject>("Background");
    go->AddComponent<dae::TextureComponent>(dae::ResourceManager::GetInstance().LoadTexture("background.tga"));
    scene.Add(go);

    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

    const auto newText = std::make_shared<dae::GameObject>("HelloWorldText");
    newText->AddComponent<dae::TextComponent>(std::string("Hello World!"), font);
    newText->GetTransform().SetWorldPosition(glm::vec3{200, 200, 0});

    scene.Add(newText);

    const auto imageTest = std::make_shared<dae::GameObject>("DaeLogo");
    auto texture = dae::ResourceManager::GetInstance().LoadTexture("logo.tga");
    imageTest->AddComponent<dae::TextureComponent>(texture);
    imageTest->GetTransform().SetWorldPosition(glm::vec3{200, 100, 0});

    scene.Add(imageTest);

    const auto fpsGameobject = std::make_shared<dae::GameObject>("FpsText");
    const auto textComponent = fpsGameobject->AddComponent<dae::TextComponent>("", font);
    const auto fpsComponent = fpsGameobject->AddComponent<dae::FPSComponent>();
    fpsComponent->SetTargetTextComponent(textComponent);

    scene.Add(fpsGameobject);

    const auto parentObj = std::make_shared<dae::GameObject>("ParentObj");
    const auto childObj = std::make_shared<dae::GameObject>("ChildObj");

    const auto childImage = dae::ResourceManager::GetInstance().LoadTexture("child.jpg");
    const auto parentImage = dae::ResourceManager::GetInstance().LoadTexture("Parent.jpg");

    parentObj->AddComponent<dae::TextureComponent>(parentImage);
    parentObj->GetTransform().SetWorldPosition(glm::vec3{200, 200, 0});
    parentObj->AddComponent<dae::Rotator>(30.f, 10.f);
    // parentObj->GetTransform().AddChild(&childObj->GetTransform());

    childObj->GetTransform().SetParent(&parentObj->GetTransform());
    childObj->AddComponent<dae::TextureComponent>(childImage);
    childObj->AddComponent<dae::Rotator>(30.f, 5.f);
    childObj->GetTransform().SetLocalPosition(glm::vec3{50, 50, 0});

    scene.Add(parentObj);
    scene.Add(childObj);

    auto ThrashTheCacheObj = std::make_shared<dae::GameObject>("ThrashTheCache");
    ThrashTheCacheObj->AddComponent<dae::ThrashTheCacheComponent>();

    scene.Add(ThrashTheCacheObj);

}

int main(int, char*[]) {
#if __EMSCRIPTEN__
	fs::path data_location = "";
#else
    fs::path data_location = "./Data/";
    if (!fs::exists(data_location))
        data_location = "../Data/";
#endif
    dae::Minigin engine(data_location);
    engine.Run(load);
    return 0;
}
