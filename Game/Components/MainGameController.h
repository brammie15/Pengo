#ifndef MAINGAMECONTROLLER_H
#define MAINGAMECONTROLLER_H
#include <map>

#include "ObjectModel/Component.h"
#include "Pengo/PengoComponent.h"
#include "SnoBee/SnoBeeComponent.h"
#include "Tile/TileComponent.h"


namespace pengo{
    class MainGameController final: public fovy::Component {
    public:
        explicit MainGameController(fovy::GameObject& pParent, PengoComponent* pengo, GridComponent* grid, SnoBeeComponent* snoBee, const std::map<fovy::Direction, fovy::GameObject*>& walls);

        void Update() override;

        void SetupBindings(GridComponent* grid);

        void AddInftestedtile(IceBlockComponent* tile) {
            m_infestedTiles.push_back(tile);
            m_onInfestedTilesChange.Invoke(static_cast<int>(m_infestedTiles.size()));
        }

        void RemoveInfestedTile(IceBlockComponent* tile) {
            auto it = std::remove(m_infestedTiles.begin(), m_infestedTiles.end(), tile);
            if (it != m_infestedTiles.end()) {
                m_infestedTiles.erase(it, m_infestedTiles.end());
            }
            m_onInfestedTilesChange.Invoke(static_cast<int>(m_infestedTiles.size()));
        }

        [[nodiscard]] const std::vector<IceBlockComponent*>& GetInfestedTiles() const { return m_infestedTiles; }

        [[nodiscard]] PengoComponent* GetPengo() const { return m_pengo; }
        [[nodiscard]] SnoBeeComponent* GetSnoBee() const { return m_snoBee; }

        fovy::Event<int>& GetOnInfestedTilesChange() { return m_onInfestedTilesChange; }

        [[nodiscard]] bool IsGameOver() const { return m_isGameOver; }
        [[nodiscard]] bool IsGameStarted() const { return m_isGameStarted; }
        [[nodiscard]] int GetScore() const { return m_score; }

        void EggBlockBroken(IceBlockComponent* tile) {
            if (tile) {
                RemoveInfestedTile(tile);
                // m_score += 100; // Increment score by 100 for each egg block broken
                m_onInfestedTilesChange.Invoke(static_cast<int>(m_infestedTiles.size()));
            }
        }


    private:
        PengoComponent* m_pengo{ nullptr };
        GridComponent* m_grid{ nullptr };
        SnoBeeComponent* m_snoBee{ nullptr };

        // Game state variables
        bool m_isGameOver{ false };
        bool m_isGameStarted{ false };
        int m_score{ 0 };

        std::vector<IceBlockComponent*> m_infestedTiles{};

        std::map<fovy::Direction, fovy::GameObject*> m_walls;

        fovy::Event<int> m_onInfestedTilesChange{};
    };
}



#endif //MAINGAMECONTROLLER_H
