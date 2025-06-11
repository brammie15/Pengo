#ifndef GRIDCOMPONENT_H
#define GRIDCOMPONENT_H

#include <vec2.hpp>
#include <vec3.hpp>
#include <vector>

#include "ObjectModel/Component.h"

class GridComponent final: public fovy::Component {
public:
    explicit GridComponent(fovy::GameObject& pParent, glm::ivec2 size, glm::vec2 cellSize);

    struct Cell {
        fovy::GameObject* occupant = nullptr; // null if empty
        bool walkable = true;
        // add tile type, cost, etc.
    };


    void Update() override;
    void Render() override;

    void ImGuiInspector() override;

    Cell& GetCell(const glm::ivec2& pos);
    [[nodiscard]] bool IsOccupied(const glm::ivec2& pos) const;
    [[nodiscard]] bool IsWalkable(const glm::ivec2 vec) const;
    void SetOccupant(const glm::ivec2& pos, fovy::GameObject* object);
    [[nodiscard]] glm::vec3 WorldPositionFromGrid(const glm::ivec2& pos) const;
    [[nodiscard]] glm::ivec2 GridPositionFromWorld(const glm::vec3& pos) const;

    [[nodiscard]] bool IsWithinBounds(const glm::ivec2& pos) const {
        return pos.x >= 0 && pos.x < m_width && pos.y >= 0 && pos.y < m_height;
    }

    [[nodiscard]] int GetHeight() const { return m_height; }
    [[nodiscard]] int GetWidth() const { return m_width; }
    [[nodiscard]] const glm::vec2& GetCellSize() const { return m_cellSize; }
    void LoadLevel(const std::string& levelFile);

private:
    std::vector<std::vector<Cell>> grid{};
    int m_width{-1};
    int m_height{-1};
    glm::vec2 m_cellSize{1, 1};

    bool m_renderDebugGrid{true};
};

#endif //GRIDCOMPONENT_H
