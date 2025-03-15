#include "ScoreComponent.h"

#include "imgui.h"

namespace fovy {
    ScoreComponent::ScoreComponent(GameObject& pParent): Component(pParent, "score") {
    }

    void ScoreComponent::AddPoints(int amount) {
        m_score += amount;
        m_ScoreChanged.Invoke(m_score);
    }

    void ScoreComponent::Update() {
    }

    void ScoreComponent::ImGuiInspector() {
        if (ImGui::TreeNode("ScoreComponent")) {
            ImGui::Text((std::string("Current Score: ") + std::to_string(m_score)).c_str());
            ImGui::PushItemWidth(300);
            ImGui::InputInt("#", &m_score);
            ImGui::TreePop();
        }
    }
} // fovy
