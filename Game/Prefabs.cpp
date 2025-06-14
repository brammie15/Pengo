#include "Prefabs.h"

#include "Components/PointsCollectedText.h"
#include "Components/SpriteRenderer.h"
#include "Components/TextComponent.h"
#include "Components/SnoBee/SnoBeeComponent.h"
#include "Components/SnoBee/SnoBeeController.h"
#include "Components/UI/LetterRoller.h"
#include "Managers/ResourceManager.h"
#include "ObjectModel/GameObject.h"

void pengo::SpawnPointsCollected(fovy::Scene& scene, glm::vec3 positon, int points) {
    const auto pointsCollectedObject = std::make_shared<fovy::GameObject>("PointsCollected");

    [[maybe_unused]] auto pointsCollectedText = pointsCollectedObject->AddComponent<fovy::TextComponent>(std::to_string(points), fovy::ResourceManager::GetInstance().LoadFont("pengo-arcade.otf", 10));

    [[maybe_unused]] auto pointsCollectedComponent = pointsCollectedObject->AddComponent<pengo::PointsCollectedText>();

    pointsCollectedObject->GetTransform().SetWorldPosition(positon);

    scene.Add(pointsCollectedObject);
}

void pengo::AddEnemy(fovy::Scene* scene, GridComponent* grid, glm::vec2 gridPos, const std::string& name) {
    const auto snoBeeObject = std::make_shared<fovy::GameObject>(name);
    const auto snoBeeSprite = snoBeeObject->AddComponent<fovy::SpriteRenderer>();
    snoBeeSprite->SetTexture("PengoSnoBee.png");
    snoBeeSprite->SetTileIndex(0);
    snoBeeSprite->AddAnimation("spawn", {0, 1, 2, 3, 4, 5}, 0.2f, false);
    snoBeeSprite->AddAnimation("idle_down", {9}, 0.5f, true);
    snoBeeSprite->AddAnimation("idle_left", {11}, 0.5f, true);
    snoBeeSprite->AddAnimation("idle_up", {13}, 0.5f, true);
    snoBeeSprite->AddAnimation("idle_right", {15}, 0.5f, true);

    snoBeeSprite->AddAnimation("walk_down", {8, 9}, 0.5f, true);
    snoBeeSprite->AddAnimation("walk_left", {10, 11}, 0.5f, true);
    snoBeeSprite->AddAnimation("walk_up", {12, 13}, 0.5f, true);
    snoBeeSprite->AddAnimation("walk_right", {14, 15}, 0.5f, true);

    snoBeeSprite->AddAnimation("frenzy_down", {16,17}, 0.5f, true);
    snoBeeSprite->AddAnimation("frenzy_left", {18,19}, 0.5f, true);
    snoBeeSprite->AddAnimation("frenzy_up", {20,21}, 0.5f, true);
    snoBeeSprite->AddAnimation("frenzy_right", {22,23}, 0.5f, true);

    snoBeeSprite->AddAnimation("stun", {6,7,6,7,6,7,6,7}, 0.5f, false);


    [[maybe_unused]] auto snoBeeComponent = snoBeeObject->AddComponent<pengo::SnoBeeComponent>(grid, gridPos);
    snoBeeObject->AddComponent<SnoBeeController>(grid);


    scene->Add(snoBeeObject);
}

void pengo::AddButtonToCanvas(const std::shared_ptr<fovy::GameObject>& button, const std::shared_ptr<fovy::GameObject>& canvas, const glm::vec3& position) {
    const glm::vec2 buttonSize = button->GetComponent<fovy::TextComponent>()->GetSize();
    // Adjust the position to center the button on the canvas
    button->GetTransform().SetLocalPosition(glm::vec3(position.x - buttonSize.x / 2, position.y - buttonSize.y / 2, position.z));
    button->GetTransform().SetParent(&canvas->GetTransform());
}

fovy::GameObject* pengo::CreateLetterRoller(fovy::Scene* scene, fovy::GameObject* canvas, glm::vec3 position, std::shared_ptr<fovy::Font> font, const std::string& name) {
    const auto letterObject = std::make_shared<fovy::GameObject>(name);
    const auto letterTextObject = std::make_shared<fovy::GameObject>(name + "Text");
    letterTextObject->AddComponent<fovy::TextComponent>("A", font);

    letterTextObject->GetTransform().SetParent(&letterObject->GetTransform());
    letterTextObject->GetTransform().SetLocalPosition(glm::vec3(18, 30, 0));

    letterObject->GetTransform().SetParent(&canvas->GetTransform());
    [[maybe_unused]] auto letter1Text = letterObject->AddComponent<pengo::LetterRoller>();
    letterObject->GetTransform().SetWorldPosition(position);

    scene->Add(letterObject);
    scene->Add(letterTextObject);

    return letterObject.get();
}

pengo::PengoComponent* pengo::CreatePlayer(fovy::Scene& scene, GridComponent* grid, const std::string& name, const glm::ivec2& position) {
    const auto Player1 = std::make_shared<fovy::GameObject>(name);

    const glm::vec3 worldPos = grid->WorldPositionFromGrid(position);
    Player1->GetTransform().SetWorldPosition(worldPos);

    const auto playerSprite = Player1->AddComponent<fovy::SpriteRenderer>();
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


    playerSprite->PlayAnimation("idle_up");

    const auto playerComponent = Player1->AddComponent<pengo::PengoComponent>(grid);

    scene.Add(Player1);

    return playerComponent;
}
