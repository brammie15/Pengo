#include "Scene.h"
#include "../ObjectModel/GameObject.h"

#include <algorithm>
#include <functional>

#include "imgui_internal.h"

#include <glm.hpp>
#include <gtc/type_ptr.hpp>

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
    for (auto& object: m_objects) {
        object->FixedUpdate();
    }
}

void Scene::LateUpdate() {
    for (auto& object: m_objects) {
        object->LateUpdate();
    }
}

void Scene::Render() const {
    for (const auto& object: m_objects) {
        object->Render();
    }
}

void Scene::RenderImgui() {
    ImGui::ShowDemoWindow();
    ImGui::Begin(std::string("Scene: " + m_name).c_str());
    ImGui::SetWindowSize(ImVec2{400, 500});

    int id = 0;

    // Recursive function to render an object and its children
    std::function<void(GameObject*)> RenderObject = [&](GameObject* object) {
        ImGui::PushID(id++);
        bool treeOpen = ImGui::TreeNodeEx(object->GetName().c_str(), ImGuiTreeNodeFlags_AllowOverlap);

        // Destroy button on the same line
        ImGui::SameLine();
        if (ImGui::SmallButton("Destroy")) {
            object->Destroy();
        }

        if (treeOpen) {
            auto& transform = object->GetTransform();

            // Transform UI
            ImGui::Text("Transform");

            glm::vec3 localPos = transform.GetLocalPosition();
            ImGui::Text("Local: ");
            ImGui::SameLine();
            if (ImGui::DragFloat3("##", glm::value_ptr(localPos))) {
                transform.SetLocalPosition(localPos);
            }

            glm::vec3 worldPos = transform.GetWorldPosition();
            ImGui::Text("World: ");
            ImGui::SameLine();
            if (ImGui::DragFloat3("##2", glm::value_ptr(worldPos))) {
                transform.SetWorldPosition(worldPos);
            }

            ImGui::SeparatorText("Object's Components");

            // Render components
            for (auto& component : object->GetComponents()) {
                ImGui::PushID(id++);
                component->ImGuiInspector();
                ImGui::PopID();
            }
            ImGui::SeparatorText("Children");
            // Render children recursively
            for (auto child : transform.GetChildren()) {
                RenderObject(child->GetOwner());
            }

            ImGui::TreePop();
        }
        ImGui::PopID();
    };

    // Start rendering the root node
    if (ImGui::TreeNode("ROOT")) {
        for (auto& object : m_objects) {
            if (!object->GetTransform().GetParent()) { // Only render root objects
                RenderObject(object.get());
            }
        }
        ImGui::TreePop();
    }

    ImGui::End();
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

    // //Strange for loop since im deleting during looping over it
    // for (auto it = m_objects.begin(); it != m_objects.end();) {
    //     if ((*it)->IsBeingDestroyed()) {
    //         it = m_objects.erase(it);
    //     } else {
    //         ++it;
    //     }
    // }

    std::erase_if(m_objects, [](const std::shared_ptr<GameObject>& gameObject) {
        return gameObject->IsBeingDestroyed();
    });
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
