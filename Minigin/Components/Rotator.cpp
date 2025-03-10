#include "Rotator.h"

#include "imgui.h"
#include "Timer.h"

fovy::Rotator::Rotator(GameObject& parent, float distance, float speed):
    Component(parent, "Rotator"),
    m_Distance(distance),
    m_Speed(speed),
    m_CurrentAngle(0),
    m_OriginalPosition(GetTransform().GetWorldPosition())
{}

void fovy::Rotator::Update() {
    m_CurrentAngle += m_Speed * static_cast<float>(Time::GetInstance().DeltaTime());
    const float x = cos(m_CurrentAngle) * m_Distance;
    const float y = sin(m_CurrentAngle) * m_Distance;
    GetTransform().SetLocalPosition(m_OriginalPosition + glm::vec3(x, y, 0));

}

void fovy::Rotator::ImGuiInspector() {
    if (ImGui::TreeNode("RotatorComponent")) {
        ImGui::Text("Current Angle: %f", m_CurrentAngle);
        ImGui::DragFloat("Radius", &m_Distance, 0.1f, 0);
        ImGui::DragFloat("Speed", &m_Speed, 0.1f, 0);
        ImGui::TreePop();
    }
}
