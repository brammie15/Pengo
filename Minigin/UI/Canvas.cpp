#include "Canvas.h"

#include <imgui.h>

#include "Focusable.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Managers/Renderer.h"
#include "ObjectModel/GameObject.h"
#include "ObjectModel/Transform.h"

namespace fovy {
    class TextComponent;
}

fovy::Canvas::Canvas(GameObject& pParent): Component{pParent, "Canvas"} {
    int x, y;
    SDL_GetWindowSize(Renderer::GetInstance().GetSDLWindow(), &x, &y);
    m_size = {static_cast<float>(x), static_cast<float>(y)};
}

void fovy::Canvas::Start() {
    BuildNavigationGraph();
}

void fovy::Canvas::Update() {
}

void fovy::Canvas::ImGuiInspector() {
    if (ImGui::TreeNode("Canvas")) {
        // Basic canvas properties
        ImGui::Checkbox("Show Debug", &m_showDebug);
        ImGui::ColorEdit4("Background Color", &m_backgroundColor.r);
        ImGui::Checkbox("Draw Connections", &m_drawConnections);
        ImGui::DragFloat2("Canvas Size", &m_size.x, 1.0f, 100.0f, 2000.0f);

        if (ImGui::Button("Recalc Navigation Graph")) {
            BuildNavigationGraph();
        }

        //Current focus gameObj
        ImGui::Text("Selected GameObject: ");
        ImGui::SameLine();
        if (m_currentFocus != nullptr) {
            ImGui::Text("%s", m_currentFocus->GetName().c_str());
        }


        ImGui::Separator();

        if (ImGui::TreeNodeEx("UI Elements", ImGuiTreeNodeFlags_DefaultOpen)) {
            for (size_t i = 0; i < m_uiElements.size(); ++i) {
                if (ImGui::TreeNode((void*)(intptr_t)i, "Element %d", i)) {
                    static bool visible = true;
                    static glm::vec2 position{};
                    static glm::vec2 size{100.0f, 50.0f};

                    ImGui::Checkbox("Visible", &visible);
                    ImGui::DragFloat2("Position", &position.x, 1.0f);
                    ImGui::DragFloat2("Size", &size.x, 1.0f, 0.0f, 1000.0f);

                    static glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};
                    ImGui::ColorEdit4("Color", &color.r);

                    ImGui::TreePop();
                }
            }

            ImGui::TreePop();
        }

        ImGui::Separator();

        ImGui::TreePop();
    }
}

void fovy::Canvas::Render() {
    if (m_drawConnections) {
        for (const auto& [from, neighbors] : m_navigationGraph) {
            const glm::vec2 fromCenter = GetScreenRect(from).GetCenter();

            for (const auto& [dir, to] : neighbors) {
                if (!to) continue;
                const glm::vec2 toCenter = GetScreenRect(to).GetCenter();

                SDL_Color color = {255, 0, 0, 255}; // Red arrows for now
                Renderer::GetInstance().RenderArrow(fromCenter, toCenter, color, 8.0f);
            }
        }
    }
}

void fovy::Canvas::Move(Direction dir) {
    if (!m_currentFocus) return;

    const auto next = m_navigationGraph[m_currentFocus][dir];
    if (next) {
        SetFocus(next);
    }
}

void fovy::Canvas::Interact() {
    m_currentFocus->GetComponent<Focusable>()->OnInteract();
}

void fovy::Canvas::SetFocus(GameObject* object) {
    if (m_currentFocus) {
        m_currentFocus->GetComponent<Focusable>()->OnDeselect();
    }
    m_currentFocus = object;
    m_currentFocus->GetComponent<Focusable>()->OnSelect();
}

void fovy::Canvas::BuildNavigationGraph() {
    const auto focusables = GetGameObject()->GetTransform().GetChildren();
    std::vector<Focusable*> focusableComponents;
    focusableComponents.reserve(focusables.size());
    for (auto& child: focusables) {
        if (auto focusable = child->GetOwner()->GetComponent<Focusable>()) {
            focusableComponents.push_back(focusable);
        }
    }

    m_uiElements.clear();
    for (const auto& focusable: focusableComponents) {
        m_uiElements.push_back(focusable->GetGameObject());
    }

    m_navigationGraph.clear();

    for (const auto& focusable: focusableComponents) {
        GameObject* obj = focusable->GetGameObject();
        m_navigationGraph[obj] = CalculateNeighbors(obj, focusableComponents);
    }

    if (!focusables.empty()) {
        SetFocus(focusables[0]->GetOwner());
    }
}

fovy::Rect fovy::Canvas::GetScreenRect(GameObject* obj) {
    if (const auto sprite = obj->GetComponent<SpriteRenderer>()) {
        glm::vec2 size = sprite->GetSize(); // Sprite's native size in pixels
        glm::vec2 position = obj->GetTransform().GetWorldPosition();
        return Rect({position.x, position.y}, {size.x, size.y});
    }

    if (const auto text = obj->GetComponent<TextComponent>()) {
        glm::vec2 size = text->GetSize();
        glm::vec2 position = obj->GetTransform().GetWorldPosition();
        return Rect({position.x, position.y}, {size.x, size.y});
    }
    return Rect{{0.0f, 0.0f}, {100.0f, 50.0f}}; // Default rect if no specific component found
}

std::unordered_map<fovy::Direction, fovy::GameObject*> fovy::Canvas::CalculateNeighbors(GameObject* source, const std::vector<Focusable*>& allFocusables) {
    std::unordered_map<Direction, GameObject*> neighbors;
    const Rect sourceRect = GetScreenRect(source);

    for (auto& target: allFocusables) {
        if (target->GetGameObject() == source) continue;

        Rect targetRect = GetScreenRect(target->GetGameObject());
        Direction dir = CalculateDirection(sourceRect, targetRect);

        if (!neighbors[dir] || IsBetterCandidate(sourceRect, targetRect, neighbors[dir])) {
            neighbors[dir] = target->GetGameObject();
        }
    }

    return neighbors;
}

fovy::Direction fovy::Canvas::CalculateDirection(const Rect& source, const Rect& target) {
    const glm::vec2 delta = target.GetCenter() - source.GetCenter();

    if (std::abs(delta.x) > std::abs(delta.y)) {
        return delta.x > 0 ? Direction::Right : Direction::Left;
    } else {
        return delta.y > 0 ? Direction::Down : Direction::Up;
    }
}

bool fovy::Canvas::IsBetterCandidate(const Rect& source, const Rect& newCandidate, GameObject* currentBest) {
    return glm::distance(source.GetCenter(), newCandidate.GetCenter()) <
           glm::distance(source.GetCenter(), GetScreenRect(currentBest).GetCenter());
}
