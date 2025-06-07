#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H
#include <vec2.hpp>

#include "../GridComponent.h"
#include "ObjectModel/Component.h"


class TileComponent final: public fovy::Component {
public:
    explicit TileComponent(fovy::GameObject& pParent, GridComponent* grid, const glm::ivec2& gridPos);
    void Update() override;

private:
    glm::ivec2 m_GridPosition{ 0, 0 };
    GridComponent* m_pGrid{ nullptr };
};


#endif //TILECOMPONENT_H
