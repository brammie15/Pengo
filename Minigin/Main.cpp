#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "ResourceManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

#include <filesystem>

#include "GameObject.h"
#include "Components/FPSComponent.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
namespace fs = std::filesystem;

void load() {
    auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

    const auto go = std::make_shared<dae::GameObject>();
    go->AddComponent<dae::TextureComponent>(dae::ResourceManager::GetInstance().LoadTexture("background.tga"));
    scene.Add(go);

    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

    const auto newText = std::make_shared<dae::GameObject>();
    newText->AddComponent<dae::TextComponent>(std::string("Hello World!"), font);
    newText->GetTransform().SetPosition(200, 200, 0);

    scene.Add(newText);

    const auto imageTest = std::make_shared<dae::GameObject>();
    auto texture = dae::ResourceManager::GetInstance().LoadTexture("logo.tga");
    imageTest->AddComponent<dae::TextureComponent>(texture);
    imageTest->GetTransform().SetPosition(200, 100, 0);

    scene.Add(imageTest);

    const auto fpsGameobject = std::make_shared<dae::GameObject>();
    const auto textComponent = fpsGameobject->AddComponent<dae::TextComponent>("", font);
    const auto fpsComponent = fpsGameobject->AddComponent<dae::FPSComponent>();
    fpsComponent->SetTargetTextComponent(textComponent);

    scene.Add(fpsGameobject);
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
