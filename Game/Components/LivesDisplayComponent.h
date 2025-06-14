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
        void Render() override;

        void OnLivesUpdate(int newLifes);


    private:
        Texture2D* m_pLifeTexture{nullptr};

        int m_Lives{3}; // Default number of lives

    };
}



#endif //LIVESDISPLAYCOMPONENT_H
