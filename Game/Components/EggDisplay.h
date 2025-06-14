#ifndef EGGDISPLAY_H
#define EGGDISPLAY_H

#include "ObjectModel/Component.h"
#include "Resources/Texture2D.h"

namespace pengo {
    class EggDisplay final: public fovy::Component {
    public:
        explicit EggDisplay(fovy::GameObject& pParent);

        void Update() override;
        void Render() override;
        void UpdateEggCount(int eggCount) {
            m_eggCount = eggCount;
        }

    private:
        fovy::Texture2D* m_eggTexture{ nullptr };

        int m_eggCount{ 3 };

    };
}


#endif //EGGDISPLAY_H
