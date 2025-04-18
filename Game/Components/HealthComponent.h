#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H
#include "Event.h"
#include "ObjectModel/Component.h"

namespace fovy {
    class HealthComponent final : public Component {
    public:
        void Update() override;

        explicit HealthComponent(GameObject& parent, int maxLifes);

        Event<int>& GetOnHealthChangeEvent(){ return m_OnHealthChange; }

        void Damage(int amount);

    private:
        Event<int> m_OnHealthChange;

        bool m_isDead{ false };
        int m_livesLeft;
    };
}


#endif //HEALTHCOMPONENT_H
