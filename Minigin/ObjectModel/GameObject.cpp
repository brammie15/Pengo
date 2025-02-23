#include "GameObject.h"
#include <string>

#include <iostream>

#include "Managers/Renderer.h"
#include "Managers/ResourceManager.h"


dae::GameObject::~GameObject() {
    std::cout << "gameobject destroyed: " << GetName() << std::endl;
}

void dae::GameObject::Update() {
    for (const auto& component: m_Components) {
        component->Update();
    }
}

void dae::GameObject::LateUpdate() {
    for (const auto& component: m_Components) {
        component->LateUpdate();
    }
}

void dae::GameObject::FixedUpdate() {
    for (const auto& component: m_Components) {
        component->FixedUpdate();
    }
}

void dae::GameObject::Render() const {
    for (const auto& component: m_Components) {
        component->Render();
    }
}

void dae::GameObject::Destroy() {
    Object::Destroy();

    for (const auto& component: m_Components) {
        component->Destroy();
    }
}

void dae::GameObject::CleanupComponents() {
    std::erase_if(m_Components, [](const std::unique_ptr<Component>& component) {
        return component->IsBeingDestroyed();
    });
}
