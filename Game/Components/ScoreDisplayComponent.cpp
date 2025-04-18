#include "ScoreDisplayComponent.h"

#include <iostream>

#include "ObjectModel/GameObject.h"

fovy::ScoreDisplayComponent::ScoreDisplayComponent(GameObject& pParent): Component(pParent, "ScoreDisplay") {
    m_textComponent = pParent.GetComponent<TextComponent>();
    if (m_textComponent == nullptr) {
        std::cerr << "ScoreDisplay can't function without a text component." << std::endl;
    }
}

void fovy::ScoreDisplayComponent::UpdateScore(int totalAmount) {
    if (m_textComponent != nullptr) {
        m_textComponent->SetText("Score: " + std::to_string(totalAmount));
    }
}

void fovy::ScoreDisplayComponent::Update() {
}
