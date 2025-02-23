#include "FPSComponent.h"

#include "Timer.h"

dae::FPSComponent::FPSComponent(GameObject &parent): Component(parent, "Fps") {
}

void dae::FPSComponent::Update() {
    m_fps = static_cast<float>(1.f / Time::GetInstance().DeltaTime());

    if (m_textComponent != nullptr) {
        m_textComponent->SetText("Fps: " + std::to_string(m_fps));
    }
}

void dae::FPSComponent::Render() {
}

void dae::FPSComponent::SetTargetTextComponent(TextComponent* component) {
    m_textComponent = component;
}
