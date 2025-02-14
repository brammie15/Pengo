#include "Component.h"

#include <stdexcept>
#include "GameObject.h"

#include "unused.h"

dae::Component::Component(dae::GameObject* pParent, const std::string& name): Object(name),
                                                                              m_ParentGameObjectPtr(pParent) {
    if (m_ParentGameObjectPtr == nullptr) {
        throw std::runtime_error("Component made with no GameObject??");
    }
}

dae::Transform& dae::Component::GetTransform() const {
    return m_ParentGameObjectPtr->GetTransform();
}

void dae::Component::Destroy() {
    // const bool isBeingDestroyed = GetIsBeingDestroyed();
    Object::Destroy();
}

void dae::Component::SetEnabled(bool enabled) {
    m_IsEnabled = enabled;
}

void dae::Component::Update() {
}

void dae::Component::LateUpdate() {
}

void dae::Component::FixedUpdate() {
}

void dae::Component::Render() {
}
