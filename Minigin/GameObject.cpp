#include <string>
#include "GameObject.h"

#include <iostream>

#include "ResourceManager.h"
#include "Renderer.h"
#include "unused.h"


dae::GameObject::~GameObject() {
    std::cout << "gameobject destroyed" << std::endl;
}

void dae::GameObject::Update() {
    for (const auto& component : m_Components) {
        component->Update();
    }
}

void dae::GameObject::LateUpdate() {
    for (const auto& component : m_Components) {
        component->LateUpdate();
    }
}

void dae::GameObject::FixedUpdate() {
    for (const auto& component : m_Components) {
        component->FixedUpdate();
    }
}

void dae::GameObject::Render() const {
    for (const auto &component: m_Components) {
        component->Render();
    }
}