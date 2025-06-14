#include "EggDisplay.h"

#include "Managers/Renderer.h"
#include "Managers/ResourceManager.h"
#include "ObjectModel/GameObject.h"

pengo::EggDisplay::EggDisplay(fovy::GameObject& pParent): Component{pParent, "eggcounter"} {
    m_eggTexture = fovy::ResourceManager::GetInstance().LoadTexture("egg.png").get();
}

void pengo::EggDisplay::Update() {
}

void pengo::EggDisplay::Render() {
    if (m_eggCount <= 0) {
        return;
    }

    const auto& pos = GetGameObject()->GetTransform().GetWorldPosition();
    constexpr float offset = 20.f;
    for (int i = 0; i < m_eggCount; ++i) {
        fovy::Renderer::GetInstance().RenderTexture(*m_eggTexture, pos.x + i * offset, pos.y, 8 * 3, 8 * 3);
    }
}
