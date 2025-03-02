#include "Component.h"

#include <stdexcept>
#include "GameObject.h"
#include "imgui.h"

dae::Component::Component(dae::GameObject& pParent, const std::string& name): Object(name), m_ParentGameObjectPtr(&pParent) {
    // if (m_ParentGameObjectPtr == nullptr) {
    //     //TODO: Change pParent ot be a reference
    //     throw std::runtime_error("Component made with no GameObject??");
    // }
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

void dae::Component::LateUpdate() {

}

void dae::Component::FixedUpdate() {
}

void dae::Component::ImGuiInspector() {

}

void dae::Component::ImGuiRender() {
}

void dae::Component::Render() {

}
