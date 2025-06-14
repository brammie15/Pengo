#include "ScoreScreenManager.h"

#include <iostream>

#include "GameController.h"
#include "Input/InputBinding.h"
#include "Input/InputManager.h"
#include "Managers/ResourceManager.h"
#include "ObjectModel/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "UI/Canvas.h"

pengo::ScoreScreenManager::ScoreScreenManager(fovy::GameObject& pParent, std::array<pengo::LetterRoller*, 3> letterRollers, fovy::Canvas* canvas): Component{pParent, "ScoreScreenManager"}, m_canvas{canvas}, m_letterRollers{std::move(letterRollers)} {
}

void pengo::ScoreScreenManager::Start() {
    m_manager.LoadFromFile("Data/Scores.txt");

    //Add textcomponents for top 5
    std::vector<std::pair<std::string, int>> topScores = m_manager.GetTopScores(5);
    for (size_t i = 0; i < topScores.size(); ++i) {
        const auto& [name, score] = topScores[i];
        const auto scoreTextObject = std::make_shared<fovy::GameObject>("ScoreText" + std::to_string(i));
        scoreTextObject->GetTransform().SetLocalPosition(glm::vec3(150, 50 + i * 30, 0));
        scoreTextObject->AddComponent<fovy::TextComponent>(name + " - " + std::to_string(score), fovy::ResourceManager::GetInstance().LoadFont("pengo-arcade.otf", 24));

        scoreTextObject->GetTransform().SetParent(&GetGameObject()->GetTransform());

        scoreTextObject->GetTransform().SetWorldPosition(
            scoreTextObject->GetTransform().GetWorldPosition() +
            glm::vec3(0, 50 + i * 30, 0) // Offset for each score text
        );

        fovy::SceneManager::GetInstance().GetScenes()[2]->Add(scoreTextObject);
    }

    m_manager.GetOnScoreChangedEvent().AddListener([this] () {
        // std::cout << "New score submitted: " << name << " - " << score << std::endl;

        for (auto& child: this->GetGameObject()->GetTransform().GetChildren()) {
            child->GetOwner()->Destroy();
        }

        std::vector<std::pair<std::string, int>> topScores = m_manager.GetTopScores(5);
        for (size_t i = 0; i < topScores.size(); ++i) {
            const auto& [name, score] = topScores[i];
            const auto scoreTextObject = std::make_shared<fovy::GameObject>("ScoreText" + std::to_string(i));
            scoreTextObject->GetTransform().SetLocalPosition(glm::vec3(150, 50 + i * 30, 0));
            scoreTextObject->AddComponent<fovy::TextComponent>(name + " - " + std::to_string(score), fovy::ResourceManager::GetInstance().LoadFont("pengo-arcade.otf", 24));

            scoreTextObject->GetTransform().SetParent(&GetGameObject()->GetTransform());

            scoreTextObject->GetTransform().SetWorldPosition(
                scoreTextObject->GetTransform().GetWorldPosition() +
                glm::vec3(0, 50 + i * 30, 0) // Offset for each score text
            );

            fovy::SceneManager::GetInstance().GetScenes()[2]->Add(scoreTextObject);
        }
    });
}

void pengo::ScoreScreenManager::Update() {
}

void pengo::ScoreScreenManager::SubmitScore(fovy::Canvas*) {
    std::string scoreName;
    for (const auto& roller: m_letterRollers) {
        scoreName += roller->GetTextComponent()->GetText();
    }

    m_manager.AddScore(scoreName, GameController::GetInstance().GetScore());

    const bool result = m_manager.SaveToFile("Data/Scores.txt");
    if (!result) {
        std::cerr << "Failed to save scores to file." << std::endl;
    } else {
        std::cout << "Scores saved successfully." << std::endl;
    }

    for (const auto& roller: m_letterRollers) {
        roller->GetGameObject()->Destroy();
    }

    m_canvas->BuildNavigationGraph();

    for (auto& child: m_canvas->GetTransform().GetChildren()) {
        child->GetOwner()->SetActive(false);
    }

    // canvas->GetGameObject()->Destroy();
}
