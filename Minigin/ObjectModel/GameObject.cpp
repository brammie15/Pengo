#include "GameObject.h"
#include <string>

#include <iostream>

#include "Managers/Renderer.h"
#include "Managers/ResourceManager.h"


fovy::GameObject::~GameObject() {
    std::cout << "gameobject destroyed: " << GetName() << std::endl;
}

fovy::GameObject::GameObject(const std::string& name): Object(name) {
}

void fovy::GameObject::Update() {
    for (const auto& component: m_Components) {
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

    for (const auto& component: m_Components) {
        component->Destroy();
    }

    for (auto child : m_TransformPtr.GetChildren()) {
        child->GetOwner()->Destroy();
    }
}

void fovy::GameObject::CleanupComponents() {
    std::erase_if(m_Components, [](const std::unique_ptr<Component>& component) {
        return component->IsBeingDestroyed();
    });
}
