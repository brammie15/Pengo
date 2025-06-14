#include "GameObject.h"
#include <string>

#include <iostream>

#include "Managers/ResourceManager.h"

fovy::GameObject::~GameObject() {
    std::cout << "gameobject destroyed: " << GetName() << std::endl;
}

void fovy::GameObject::SetActiveDirty() {
    m_ActiveDirty = true;
    for (const Transform* child : m_TransformPtr.GetChildren()) {
        child->GetOwner()->SetActiveDirty();
    }
}

void fovy::GameObject::UpdateActiveState() {
    const auto* parentPtr = m_TransformPtr.GetParent();

    if(parentPtr == nullptr) {
        m_ActiveInHierarchy = m_Active;
    } else {
        m_ActiveInHierarchy = m_Active && parentPtr->GetOwner()->IsActiveInHierarchy();
    }

    m_ActiveDirty = false;
}

bool fovy::GameObject::IsActiveInHierarchy() {
    if (m_ActiveDirty) {
        UpdateActiveState();
    }

    return m_ActiveInHierarchy;
}

fovy::GameObject::GameObject(const std::string& name): Object(name) {
}

void fovy::GameObject::Update() {
    for (const auto& component: m_Components) {
        if (!component->HasStarted) {
            component->Start();
            component->HasStarted = true;
        }
        component->Update();
    }
}

void fovy::GameObject::LateUpdate() {
    for (const auto& component: m_Components) {
        component->LateUpdate();
    }
}

void fovy::GameObject::FixedUpdate() {
    for (const auto& component: m_Components) {
        component->FixedUpdate();
    }
}

void fovy::GameObject::Render() const {
    for (const auto& component: m_Components) {
        component->Render();
    }
}

void fovy::GameObject::ImGuiRender() {
    for (const auto& component: m_Components) {
        component->ImGuiRender();
    }
}

void fovy::GameObject::Destroy() {
    Object::Destroy();

    m_TransformPtr.SetParent(nullptr);

    for (const auto& component: m_Components) {
        component->Destroy();
    }

    for (const auto child : m_TransformPtr.GetChildren()) {
        child->GetOwner()->Destroy();
    }
}

void fovy::GameObject::CleanupComponents() {
    std::erase_if(m_Components, [](const std::unique_ptr<Component>& component) {
        return component->IsBeingDestroyed();
    });
}
