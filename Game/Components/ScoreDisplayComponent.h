#ifndef SCOREDISPLAYCOMPONENT_H
#define SCOREDISPLAYCOMPONENT_H
#include "Event.h"
#include "TextComponent.h"
#include "ObjectModel/Component.h"

namespace fovy {

class ScoreDisplayComponent final: public Component, public EventListener {
public:
    explicit ScoreDisplayComponent(GameObject& pParent);

    void UpdateScore(int totalAmount);
    void Update() override;

private:

    TextComponent* m_textComponent{ nullptr };
};

}

#endif //SCOREDISPLAYCOMPONENT_H
