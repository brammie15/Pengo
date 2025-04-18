#include "HealthComponent.h"

void fovy::HealthComponent::Update() {
}

fovy::HealthComponent::HealthComponent(GameObject& parent, int maxLifes): Component(parent, "Health"), m_livesLeft(maxLifes) {
}

void fovy::HealthComponent::Damage(int amount) {
    m_livesLeft -= amount;
    if (m_livesLeft <= 0) {
        m_isDead = true;
        m_livesLeft = 0;
    }

    m_OnHealthChange.Invoke(m_livesLeft);
}
