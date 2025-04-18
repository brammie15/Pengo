#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H
#include "Event.h"
#include "ObjectModel/Component.h"

namespace fovy {

class ScoreComponent final: public Component {
public:
    explicit ScoreComponent(GameObject& pParent);

    void AddPoints(int amount);
    [[nodiscard]] int GetScore() const { return m_score; }

    auto& GetScoreChangedEvent(){ return m_ScoreChanged; }
    void Update() override;
    void ImGuiInspector() override;

private:
    Event<int> m_ScoreChanged;
    int m_score{ 0 };
};

} // fovy

#endif //SCORECOMPONENT_H
