#ifndef LIVESDISPLAYCOMPONENT_H
#define LIVESDISPLAYCOMPONENT_H

#include "Event.h"
#include "../../Minigin/Components/TextComponent.h"
#include "ObjectModel/Component.h"

namespace fovy {
    class LivesDisplayComponent final: public Component, public EventListener  {
    public:

        explicit LivesDisplayComponent(fovy::GameObject& pParent);
        void Update() override;

        void OnLivesUpdate(int newLifes);


    private:

        TextComponent* m_textComponent{ nullptr };
    };
}



#endif //LIVESDISPLAYCOMPONENT_H
