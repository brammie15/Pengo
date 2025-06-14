#include "LivesDisplayComponent.h"

#include <iostream>

#include "Managers/Renderer.h"
#include "Managers/ResourceManager.h"
#include "ObjectModel/GameObject.h"

fovy::LivesDisplayComponent::LivesDisplayComponent(fovy::GameObject& pParent): Component(pParent, "LivesComponent") {
    m_pLifeTexture = ResourceManager::GetInstance().LoadTexture("lives.png").get();
}

void fovy::LivesDisplayComponent::Update() {
}

void fovy::LivesDisplayComponent::Render() {
    if (m_Lives <= 0) return;

    const auto basePos = GetGameObject()->GetTransform().GetWorldPosition();
    for (int i = 0; i < m_Lives; ++i) {
        Renderer::GetInstance().RenderTexture(*m_pLifeTexture, basePos.x + i * 34, basePos.y, 32, 32);
    }
}

void fovy::LivesDisplayComponent::OnLivesUpdate(int newLifes) {
    m_Lives = newLifes;
}
