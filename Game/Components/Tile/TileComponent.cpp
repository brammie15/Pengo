#include "TileComponent.h"

#include <iostream>

#include "ObjectModel/Transform.h"

TileComponent::TileComponent(fovy::GameObject& pParent, GridComponent* grid, const glm::ivec2& gridPos):
    Component(pParent, "TileComponent"),
    m_GridPosition{gridPos},
    m_pGrid{grid}
{
    if (m_pGrid->IsOccupied(gridPos)) {
        std::cerr << "TileComponent: Grid position already occupied!" << std::endl;
        return;
    }

    m_pGrid->SetOccupant(m_GridPosition, &pParent);
    GetTransform().SetWorldPosition(m_pGrid->WorldPositionFromGrid(m_GridPosition));
}

void TileComponent::Update() {
}


