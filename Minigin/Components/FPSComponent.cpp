#include "FPSComponent.h"

#include "Timer.h"

fovy::FPSComponent::FPSComponent(GameObject &parent): Component(parent, "Fps") {
}

void fovy::FPSComponent::Update() {
    m_fps = static_cast<float>(1.f / Time::GetInstance().DeltaTime());

    if (m_textComponent != nullptr) {
        m_textComponent->SetText("Fps: " + std::to_string(m_fps));
    }
}

void fovy::FPSComponent::Render() {
}

void fovy::FPSComponent::SetTargetTextComponent(TextComponent* component) {
    m_textComponent = component;
}
