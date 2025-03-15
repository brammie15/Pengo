#include "LivesDisplayComponent.h"

#include <iostream>

#include "ObjectModel/GameObject.h"

fovy::LivesDisplayComponent::LivesDisplayComponent(fovy::GameObject& pParent): Component(pParent, "LivesComponent") {
    m_textComponent = pParent.GetComponent<TextComponent>();
    if (m_textComponent == nullptr) {
        std::cerr << "Lives can't function without a text component." << std::endl;
    }
}

void fovy::LivesDisplayComponent::Update() {
}

void fovy::LivesDisplayComponent::OnLivesUpdate(int newLifes) {
    if (m_textComponent != nullptr) {
        m_textComponent->SetText("Lives Left: " + std::to_string(newLifes));
    }
}
