#include "GameObject.h"
#include <string>

#include <iostream>

#include "Renderer.h"
#include "ResourceManager.h"
#include "unused.h"


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
    //Strange for loop since im deleting during looping over it
    for (auto it = m_Components.begin(); it != m_Components.end();) {
        if ((*it)->IsBeingDestroyed()) {
            it = m_Components.erase(it);
        } else {
            ++it;
        }
    }
}
