#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {
}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object) {
    m_objects.emplace_back(std::move(object));
}

void Scene::Remove(const std::shared_ptr<GameObject>& object) {
    std::erase(m_objects, object);
}

void Scene::RemoveAll() {
    m_objects.clear();
}

void Scene::Update() {
    for (auto& object: m_objects) {
        object->Update();
    }
}

void Scene::FixedUpdate() {
    for (auto& object : m_objects) {
        object->FixedUpdate();
    }
}

void Scene::LateUpdate() {
    for (auto& object : m_objects) {
        object->LateUpdate();
    }
}

void Scene::Render() const {
    for (const auto& object: m_objects) {
        object->Render();
    }
}

void Scene::CleanupDestroyedGameObjects() {

    if (m_BeingUnloaded) {
        //Scene is gone anyways, kill everything
        m_objects.clear();
        return;
    }

    for (auto& gameObject: m_objects) {
        //First check if a gameobjects components needs to be destroyed
        gameObject->CleanupComponents();
    }

    //Strange for loop since im deleting during looping over it
    for (auto it = m_objects.begin(); it != m_objects.end();) {
        if ((*it)->isBeingDestroyed()) {
            it = m_objects.erase(it);
        } else {
            ++it;
        }
    }
}

void Scene::Unload() {
    m_BeingUnloaded = true;
}

void Scene::DestroyGameObjects() {
    if (m_BeingUnloaded) {
        //Scene is gone anyways, kill everything
        for (auto& gameObject: m_objects) {
            gameObject->Destroy();
        }
    } else {
        assert(true && "Scene is being cleared but not unloaded? Wierd");
    }
}
