#include "LetterRoller.h"

#include <iostream>

#include "Timer.h"
#include "Managers/Renderer.h"
#include "UI/Canvas.h"

pengo::LetterRoller::LetterRoller(fovy::GameObject& pParent): Focusable{pParent, "LetterRoller"} {
    m_textComponent = GetGameObject()->GetComponentInChildren<fovy::TextComponent>();
    if (m_textComponent == nullptr) {
        std::cerr << "TextComponent not found in LetterRoller" << std::endl;
        return;
    }

    m_textComponent->SetColor({255, 255, 255, 255}); // White color
    m_textComponent->SetText(m_currentLetter);
}

void pengo::LetterRoller::Start() {

}

void pengo::LetterRoller::Update() {
    m_blinkTimer += static_cast<float>(fovy::Time::GetInstance().DeltaTime());

    if (m_blinkTimer >= m_blinkInterval) {
        m_blinking = !m_blinking; // Toggle blinking state
        m_blinkTimer = 0.0f; // Reset timer
    }
}

void pengo::LetterRoller::Render() {
    //Render a rect
    auto& transform = GetGameObject()->GetTransform();
    const glm::vec3 position = transform.GetWorldPosition();
    const glm::vec3 scale = transform.GetWorldScale();


    //if selected Underline
    if (m_selected && m_blinking) {
        glm::vec2 size = GetSize();
        float fullWidth = size.x * scale.x;
        float renderWidth = fullWidth * 0.75f;
        float height = 10.f;

        // Position the underline centered horizontally and near the bottom
        glm::vec2 center = {
            position.x + fullWidth / 2.0f,
            position.y + size.y * scale.y - height / 2.0f
        };

        fovy::Renderer::GetInstance().RenderRect(
            center.x - renderWidth / 2.0f, center.y,
            renderWidth, height,
            {255, 255, 0, 255}
        );
    }

    m_textComponent->Render();
}

void pengo::LetterRoller::OnSelect() {
    m_selected = true;
    m_blinking = true;
    m_blinkTimer = 0.0f;
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
