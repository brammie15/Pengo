#include "Component.h"

#include <stdexcept>
#include "GameObject.h"
#include "imgui.h"

fovy::Component::Component(fovy::GameObject& pParent, const std::string& name): Object(name), m_ParentGameObjectPtr(&pParent) {
    // if (m_ParentGameObjectPtr == nullptr) {
    //     //TODO: Change pParent ot be a reference
    //     throw std::runtime_error("Component made with no GameObject??");
    // }
}

fovy::Transform& fovy::Component::GetTransform() const {
    return m_ParentGameObjectPtr->GetTransform();
}

void fovy::Component::Destroy() {
    // const bool isBeingDestroyed = GetIsBeingDestroyed();
    Object::Destroy();
}

void fovy::Component::Start() {
}

void fovy::Component::SetEnabled(bool enabled) {
    m_IsEnabled = enabled;
}

void fovy::Component::LateUpdate() {
}

void fovy::Component::FixedUpdate() {
}

void fovy::Component::ImGuiInspector() {
}

void fovy::Component::ImGuiRender() {
}

void fovy::Component::Render() {

}
