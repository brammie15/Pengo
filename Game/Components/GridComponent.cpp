#include "GridComponent.h"

#include <iostream>

#include "Managers/Renderer.h"
#include "ObjectModel/GameObject.h"

void GridComponent::Initialize(int width, int height, float cellSize) {
    this->m_width = width;
    this->m_height = height;
    this->m_cellSize = cellSize;

    grid.resize(width);
    for (int x = 0; x < width; ++x) {
        grid[x].resize(height);
        for (int y = 0; y < height; ++y) {
            grid[x][y].occupant = nullptr;
            grid[x][y].walkable = true; // Default to walkable
        }
    }
}

GridComponent::Cell& GridComponent::GetCell(const glm::ivec2& pos) {
    return grid[pos.x][pos.y];
}

bool GridComponent::IsOccupied(const glm::ivec2& pos) const {
    return grid[pos.x][pos.y].occupant != nullptr;
}

void GridComponent::SetOccupant(const glm::ivec2& pos, fovy::GameObject* object) {
    grid[pos.x][pos.y].occupant = object;
}

glm::vec3 GridComponent::WorldPositionFromGrid(const glm::ivec2& pos) const {
    auto offset = GetGameObject()->GetTransform().GetWorldPosition();
    return {offset.x + pos.x * m_cellSize, offset.y + pos.y * m_cellSize, 0.f};
}

glm::ivec2 GridComponent::GridPositionFromWorld(const glm::vec3& pos) const {
    auto offset = GetGameObject()->GetTransform().GetWorldPosition();
    int x = static_cast<int>((pos.x - offset.x) / m_cellSize);
    int y = static_cast<int>((pos.y - offset.y) / m_cellSize);
    return {x, y};
}

void GridComponent::Update() {
}

void GridComponent::Render() {
    //Render the grid
    glm::vec3 offset = GetGameObject()->GetTransform().GetWorldPosition();

    for (int x = 0; x < m_width; ++x) {
        for (int y = 0; y < m_height; ++y) {
            fovy::Renderer::GetInstance().RenderRect(
                offset.x + x * m_cellSize, offset.y + y * m_cellSize, m_cellSize,
                m_cellSize, {255, 255, 255, 255}, false);
        }
    }
}

void GridComponent::ImGuiInspector() {
    if (ImGui::TreeNode("GridComponent")) {
        ImGui::Text("Grid Size: ");
        ImGui::SameLine();
        ImGui::Text("X: %d, Y: %d", m_width, m_height);
        ImGui::Text("Cell Size: ");
        ImGui::SameLine();
        ImGui::Text("%f", m_cellSize);
        ImGui::Separator();

        // Configuration
        const ImVec2 cellSize(15.0f, 15.0f); // Size of each cell in pixels
        const ImVec2 padding(2.0f, 2.0f);    // Spacing between cells

        // Start at cursor position
        ImVec2 cursorPos = ImGui::GetCursorScreenPos();

        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                ImGui::PushID(y * m_width + x);

                ImGui::SetCursorScreenPos(ImVec2(
                    cursorPos.x + x * (cellSize.x + padding.x),
                    cursorPos.y + y * (cellSize.y + padding.y)
                ));

                ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Default white color
                if (grid[y][x].occupant == nullptr) {
                    color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f); // Gray for empty cells
                } else {
                    color = ImVec4(0.0f, 0.0f, 1.0f, 1.0f); // Blue for occupied cells
                }

                ImGui::PushStyleColor(ImGuiCol_Button, color);
                if (ImGui::Button("", cellSize)) {
                    if (grid[y][x].occupant != nullptr) {
                        grid[y][x].occupant->Destroy();
                    }
                }
                ImGui::PopStyleColor();

                if (ImGui::IsItemHovered()) {
                    std::string occupantName = grid[y][x].occupant ? grid[y][x].occupant->GetName() : "None";
                    ImGui::SetTooltip("Cell (%d, %d)\nOccupant: %s", x, y, occupantName.c_str());
                }

                ImGui::PopID();
            }
        }

        ImGui::SetCursorScreenPos(ImVec2(
            cursorPos.x,
            cursorPos.y + m_height * (cellSize.y + padding.y)
        ));
        ImGui::Dummy(ImVec2(0, 0)); // Ensures proper layout continuation


        ImGui::TreePop();
    }
}
