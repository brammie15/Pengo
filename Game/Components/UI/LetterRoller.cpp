#include "LetterRoller.h"

#include <iostream>

#include "Managers/Renderer.h"
#include "UI/Canvas.h"

pengo::LetterRoller::LetterRoller(fovy::GameObject& pParent): Focusable{pParent, "LetterRoller"} {
    m_textComponent = GetGameObject()->GetComponentInChildren<fovy::TextComponent>();
    if (m_textComponent == nullptr) {
        std::cerr << "TextComponent not found in LetterRoller" << std::endl;
        return;
    }

    m_textComponent->SetColor({0, 255, 255, 255}); // White color
    m_textComponent->SetText(m_currentLetter);
}

void pengo::LetterRoller::Start() {

}

void pengo::LetterRoller::Update() {
}

void pengo::LetterRoller::Render() {
    //Render a rect
    auto& transform = GetGameObject()->GetTransform();
    const glm::vec3 position = transform.GetWorldPosition();
    const glm::vec3 scale = transform.GetWorldScale();
    fovy::Renderer::GetInstance().RenderRect(
        position.x - scale.x / 2.0f, position.y - scale.y / 2.0f,
        scale.x * 75, scale.y * 100,
        {255, 255, 255, 255}, // White color
        true
    );

    if (m_selected) {
        fovy::Renderer::GetInstance().RenderRect(
            position.x - scale.x / 2.0f, position.y - scale.y / 2.0f,
            scale.x * 75, scale.y * 100,
            {255, 0, 0, 255}, // Red border
            false
        );
    }

    m_textComponent->Render();
}

void pengo::LetterRoller::OnSelect() {
    m_selected = true;
}

void pengo::LetterRoller::OnDeselect() {
    m_selected = false;
}

bool pengo::LetterRoller::OnMove(fovy::Direction dir) {
    if (dir == fovy::Direction::Left || dir == fovy::Direction::Right) {
        return false;
    }

    if (dir == fovy::Direction::Up) {
        if (m_currentLetter == "A") {
            m_currentLetter = "Z";
        } else {
            m_currentLetter[0]--;
        }
    } else if (dir == fovy::Direction::Down) {
        if (m_currentLetter == "Z") {
            m_currentLetter = "A";
        } else {
            m_currentLetter[0]++;
        }
    }

    m_textComponent->SetText(m_currentLetter);
    return true;
}
