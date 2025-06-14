#ifndef CANVAS_H
#define CANVAS_H
#include <unordered_map>
#include <vec2.hpp>
#include <vec4.hpp>

#include "Direction.h"
#include "Event.h"
#include "Focusable.h"
#include "Components/SpriteRenderer.h"
#include "ObjectModel/Component.h"
#include "ObjectModel/GameObject.h"

namespace fovy {


struct Rect {
    glm::vec2 position;
    glm::vec2 size;

    [[nodiscard]] glm::vec2 GetCenter() const {
        return position + (size * 0.5f);
    }
};

class Canvas final: public Component {
public:
    explicit Canvas(GameObject& pParent);

    void Start() override;
    void Update() override;
    void ImGuiInspector() override;

    void Render() override;

    void Move(Direction dir);

    void Interact();
    void BuildNavigationGraph();

    Event<GameObject*>& GetOnFocusChanged() { return m_onFocusChanged; }

    [[nodiscard]] bool IsDebugEnabled() const { return m_showDebug; }
    [[nodiscard]] bool IsDebugGridEnabled() const { return m_showDebugGrid; }
    [[nodiscard]] glm::vec4 GetBackgroundColor() const { return m_backgroundColor; }
    [[nodiscard]] glm::vec2 GetSize() const { return m_size; }
    [[nodiscard]] bool ShouldDrawConnections() const { return m_drawConnections; }

private:
    void SetFocus(GameObject* object);

    std::unordered_map<GameObject*, std::unordered_map<Direction, GameObject*>> m_navigationGraph;
    GameObject* m_currentFocus = nullptr;

    std::vector<GameObject*> m_uiElements;

    bool m_showDebug{ false };
    bool m_showDebugGrid{ false };
    glm::vec4 m_backgroundColor{ 0.0f, 0.0f, 0.0f, 0.5f }; // Semi-transparent black
    glm::vec2 m_size{};
    bool m_drawConnections{ false };

    Event<GameObject*> m_onFocusChanged;


    Rect GetScreenRect(GameObject* obj);

    std::unordered_map<Direction, GameObject*> CalculateNeighbors(GameObject* source, const std::vector<Focusable*>& allFocusables);

    static Direction CalculateDirection(const Rect& source, const Rect& target);

    bool IsBetterCandidate(const Rect& source, const Rect& newCandidate, GameObject* currentBest);
};
}



#endif //CANVAS_H
