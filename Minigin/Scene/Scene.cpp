#include "Scene.h"
#include "../ObjectModel/GameObject.h"

#include <algorithm>
#include <functional>
#include <SDL_scancode.h>

#include "imgui_internal.h"

#include <glm.hpp>
#include <gtc/type_ptr.hpp>

#include "Input/InputManager.h"
#include "Managers/Renderer.h"


using namespace fovy;

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

void Scene::Load() {
    if (m_registerBindings) {
        m_registerBindings();
    }
}

void Scene::Update() {
    if (InputManager::GetInstance().IsKeyPressed(SDL_SCANCODE_F1)) {
        m_ShowDemoWindow = !m_ShowDemoWindow;
    }

    for (auto& object: m_objects) {
        if (object->IsActiveInHierarchy()) {
            object->Update();
        }
    }
}

void Scene::FixedUpdate() {
    for (auto& object: m_objects) {
        if (object->IsActiveInHierarchy()) {
            object->FixedUpdate();
        }
    }
}

void Scene::LateUpdate() {
    for (auto& object: m_objects) {
        if (object->IsActiveInHierarchy()) {
            object->LateUpdate();
        }
    }
}

void Scene::Render() const {
    for (const auto& object: m_objects) {
        if (object->IsActiveInHierarchy()) {
            object->Render();
        }
    }

    // int width, height;
    // SDL_GetWindowSize(Renderer::GetInstance().GetSDLWindow(), &width, &height);
    //
    // Renderer::GetInstance().RenderLine(
    //     static_cast<float>(width / 2), 0,
    //     static_cast<float>(width / 2), static_cast<float>(height), SDL_Color(255, 0, 0, 255) // Red vertical line
    // );
    //
    // Renderer::GetInstance().RenderLine(
    //     0, static_cast<float>(height / 2), static_cast<float>(width), static_cast<float>(height / 2), SDL_Color(0, 255, 0, 255) // Green horizontal line
    // );
}

void Scene::RenderImgui() {
    for (auto& object: m_objects) {
        object->ImGuiRender();
    }

    if (m_ShowDemoWindow) {
        ImGui::ShowDemoWindow();
    }
    ImGui::Begin(std::string("Scene: " + m_name).c_str());
    ImGui::SetWindowSize(ImVec2{500, 600});

    int id = 0;

    // Recursive function to render an object and its children
    std::function<void(GameObject*)> RenderObject = [&] (GameObject* object) {
        ImGui::PushID(id++);
        bool treeOpen = ImGui::TreeNodeEx(object->GetName().c_str(), ImGuiTreeNodeFlags_AllowOverlap);

        // Destroy button on the same line
        ImGui::SameLine();
        if (ImGui::SmallButton("Destroy")) {
            object->Destroy();
        }


        if (treeOpen) {
            auto& transform = object->GetTransform();

            bool isActive = object->IsActiveInHierarchy();
            if (ImGui::Checkbox("Active", &isActive)) {
                object->SetActive(isActive);
            }

            // Transform UI
            ImGui::Text("Transform");

            ImGui::Text("Position");
            glm::vec3 localPos = transform.GetLocalPosition();
            ImGui::Text("Local: ");
            ImGui::SameLine();
            if (ImGui::DragFloat3("#1", glm::value_ptr(localPos))) {
                transform.SetLocalPosition(localPos);
            }

            glm::vec3 worldPos = transform.GetWorldPosition();
            ImGui::Text("World: ");
            ImGui::SameLine();
            if (ImGui::DragFloat3("##2", glm::value_ptr(worldPos))) {
                transform.SetWorldPosition(worldPos);
            }

            constexpr float scaleSpeed{0.05f};

            ImGui::Text("Scale");
            glm::vec3 localScale = transform.GetLocalScale();
            ImGui::Text("Local: ");
            ImGui::SameLine();
            if (ImGui::DragFloat3("###3", glm::value_ptr(localScale), scaleSpeed)) {
                transform.SetLocalScale(localScale);
            }

            glm::vec3 worldScale = transform.GetWorldScale();
            ImGui::Text("World: ");
            ImGui::SameLine();
            if (ImGui::DragFloat3("####4", glm::value_ptr(worldScale), scaleSpeed)) {
                transform.SetWorldScale(worldScale);
            }

            ImGui::SeparatorText("Object's Components");

            // Render components
            for (auto& component: object->GetComponents()) {
                ImGui::PushID(id++);
                component->ImGuiInspector();
                ImGui::PopID();
            }
            ImGui::SeparatorText("Children");
            // Render children recursively
            for (auto child: transform.GetChildren()) {
                RenderObject(child->GetOwner());
            }

            ImGui::TreePop();
        }
        ImGui::PopID();
    };

    ImGui::Checkbox("Show Demo Window", &m_ShowDemoWindow);
    // Start rendering the root node
    if (ImGui::TreeNode("ROOT")) {
        for (auto& object: m_objects) {
            if (!object->GetTransform().GetParent()) {
                // Only render root objects
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

    std::erase_if(m_objects, [] (const std::shared_ptr<GameObject>& gameObject) {
        return gameObject->IsBeingDestroyed();
    });
}

void Scene::Unload() {
    if (m_unregisterBindings) {
        m_unregisterBindings();
    }
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
