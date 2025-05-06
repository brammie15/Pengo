#ifndef GRIDCOMPONENT_H
#define GRIDCOMPONENT_H

#include <vec2.hpp>
#include <vec3.hpp>
#include <vector>

#include "ObjectModel/Component.h"

class GridComponent final: public fovy::Component {
public:
    explicit GridComponent(fovy::GameObject& pParent)
        : Component{pParent, "GridComponent"} {
    }

    struct Cell {
        fovy::GameObject* occupant = nullptr; // null if empty
        bool walkable = true;
        // add tile type, cost, etc.
    };


    void Update() override;
    void Render() override;

    void ImGuiInspector() override;

    void Initialize(int width, int height, float cellSize);
    Cell& GetCell(const glm::ivec2& pos);
    bool IsOccupied(const glm::ivec2& pos) const;
    void SetOccupant(const glm::ivec2& pos, fovy::GameObject* object);
    glm::vec3 WorldPositionFromGrid(const glm::ivec2& pos) const;
    glm::ivec2 GridPositionFromWorld(const glm::vec3& pos) const;

    bool IsWithinBounds(const glm::ivec2& pos) const {
        return pos.x >= 0 && pos.x < m_width && pos.y >= 0 && pos.y < m_height;
    }

    int GetHeight() const { return m_height; }
    int GetWidth() const { return m_width; }
    float GetCellSize() const { return m_cellSize; }

private:
    std::vector<std::vector<Cell>> grid{};
    int m_width{-1};
    int m_height{-1};
    float m_cellSize{ 1.f };
};


#endif //GRIDCOMPONENT_H
