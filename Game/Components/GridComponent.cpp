#include "GridComponent.h"

#include <fstream>
#include <iostream>

#include "Tile/IceBlockComponent.h"
#include "SpriteRenderer.h"
#include "Texture2D.h"
#include "Tile/TileComponent.h"
#include "Managers/Renderer.h"
#include "Managers/ResourceManager.h"
#include "ObjectModel/GameObject.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

GridComponent::GridComponent(fovy::GameObject& pParent, glm::ivec2 size, const glm::vec2 gridSize): Component{pParent, "GridComponent"} {
    this->m_width = size.x;
    this->m_height = size.y;
    this->m_cellSize = gridSize;

    grid.resize(m_width);
    for (int x = 0; x < m_width; ++x) {
        grid[x].resize(m_height);
        for (int y = 0; y < m_height; ++y) {
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

bool GridComponent::IsWalkable(const glm::ivec2 vec) const { return grid[vec.x][vec.y].walkable; }

void GridComponent::SetOccupant(const glm::ivec2& pos, fovy::GameObject* object) {
    grid[pos.x][pos.y].occupant = object;
}

glm::vec3 GridComponent::WorldPositionFromGrid(const glm::ivec2& pos) const {
    const auto offset = GetGameObject()->GetTransform().GetWorldPosition();
    return {offset.x + static_cast<float>(pos.x) * m_cellSize.x, offset.y + static_cast<float>(pos.y) * m_cellSize.y, 0.f};
}

glm::ivec2 GridComponent::GridPositionFromWorld(const glm::vec3& pos) const {
    const auto offset = GetGameObject()->GetTransform().GetWorldPosition();
    int x = static_cast<int>((pos.x - offset.x) / m_cellSize.x);
    int y = static_cast<int>((pos.y - offset.y) / m_cellSize.y);
    return {x, y};
}

void GridComponent::LoadLevel(const std::string& filename) {
    std::ifstream inFile("Data/" + filename);
    if (!inFile) {
        std::cerr << "Failed to open level file: " << filename << std::endl;
        return;
    }

    static const std::unordered_map<int, std::string> tilePrefabMap = {
        {1, "WallTile"},
        {2, "Enemy"},
        {3, "GrassTile"},
        {4, "WaterTile"},
        // Extend as needed
    };

    std::string line;
    int y = 0;

    std::vector<glm::ivec2> toAddTiles{};

    while (std::getline(inFile, line) && y < m_height) {
        std::istringstream ss(line);
        int x = 0, tileId;

        while (ss >> tileId && x < m_width) {
            const glm::ivec2 cellPos{x, y};

            switch (tileId) {
                case 0:
                    break;
                case 1: {
                    toAddTiles.push_back(cellPos);
                    break;
                }
            }

            std::cout << "Processed: " << cellPos.x << " : " << cellPos.y << std::endl;


            ++x;
        }

        ++y;
    }

    inFile.close();
    auto& sm = fovy::SceneManager::GetInstance();

    for (auto tilePos: toAddTiles) {
        auto tileObject = std::make_shared<fovy::GameObject>("Tile");
        [[maybe_unused]] auto tileComponent = tileObject->AddComponent<TileComponent>(this, tilePos);
        [[maybe_unused]] auto iceBlockComponent = tileObject->AddComponent<pengo::IceBlockComponent>(this);
        auto tileSprite = tileObject->AddComponent<fovy::SpriteRenderer>();
        tileSprite->SetTexture("PengoBlocks.png");
        tileSprite->SetTileIndex(0);

        tileSprite->GetTransform().SetParent(&this->GetGameObject()->GetTransform());


        sm.GetCurrentScene().Add(tileObject);
    }

    std::cout << "Added all tiles" << std::endl;
}

void GridComponent::Update() {
}

void GridComponent::Render() {
    const glm::vec3 offset = GetGameObject()->GetTransform().GetWorldPosition();

    for (int x = 0; x < m_width; ++x) {
        for (int y = 0; y < m_height; ++y) {
            fovy::Renderer::GetInstance().RenderRect(
                offset.x + static_cast<float>(x) * m_cellSize.x, offset.y + y * m_cellSize.y, m_cellSize.x,
                m_cellSize.y, {255, 255, 255, 255}, false);
        }
    }
}

void GridComponent::ImGuiInspector() {
    if (ImGui::TreeNode("GridComponent")) {
        ImGui::Text("Grid Size: X: %d, Y: %d", m_width, m_height);
        ImGui::Text("Cell Size: X: %.1f, Y: %.1f", m_cellSize.x, m_cellSize.y);
        ImGui::Separator();

        // Grid layout settings
        constexpr ImVec2 cellSize(15.0f, 15.0f);
        constexpr ImVec2 padding(2.0f, 2.0f);
        const ImVec2 cursorStart = ImGui::GetCursorScreenPos();

        for (int y = 0; y < m_height; ++y) {
            for (int x = 0; x < m_width; ++x) {
                ImGui::PushID(y * m_width + x);

                ImVec2 cellPos = {
                    cursorStart.x + x * (cellSize.x + padding.x),
                    cursorStart.y + y * (cellSize.y + padding.y)
                };
                ImGui::SetCursorScreenPos(cellPos);

                const auto& cell = grid[x][y];

                ImVec4 color;
                if (cell.occupant != nullptr) {
                    if (cell.occupant->GetName() == "Player1") { //TODO: remove debug bad >:(
                        color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);  // Player: Green
                    } else {
                        color = ImVec4(0.2f, 0.6f, 1.0f, 1.0f);  // Occupied: Blue
                    }
                } else if (!cell.walkable) {
                    color = ImVec4(0.7f, 0.1f, 0.1f, 1.0f);  // Unwalkable: Red
                } else {
                    color = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);  // Walkable and empty: Gray
                }

                ImGui::PushStyleColor(ImGuiCol_Button, color);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);

                if (ImGui::Button("##cell", cellSize)) {
                    // Optional: Toggle walkability on click
                    // grid[x][y].walkable = !grid[x][y].walkable;
                }

                if (ImGui::IsItemHovered()) {
                    std::string occupantName = cell.occupant ? cell.occupant->GetName() : "None";
                    ImGui::SetTooltip("Cell (%d, %d)\nOccupant: %s\nWalkable: %s",
                        x, y, occupantName.c_str(), cell.walkable ? "Yes" : "No");
                }

                ImGui::PopStyleColor(3);
                ImGui::PopID();
            }
        }

        // Ensure layout doesn't overlap with following widgets
        ImGui::SetCursorScreenPos({
            cursorStart.x,
            cursorStart.y + m_height * (cellSize.y + padding.y)
        });
        ImGui::Dummy({1.0f, 1.0f});

        ImGui::TreePop();
    }
}
