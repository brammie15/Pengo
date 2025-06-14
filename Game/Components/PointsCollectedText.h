#ifndef POINTSCOLLECTEDTEXT_H
#define POINTSCOLLECTEDTEXT_H
#include "ObjectModel/Component.h"


namespace pengo {
    class PointsCollectedText final: public fovy::Component {
    public:
        explicit PointsCollectedText(fovy::GameObject& pParent);
        void Update() override;

    private:
        float m_moveTimer{0.0f};
        float m_moveDuration{1.f};
    };
}


#endif //POINTSCOLLECTEDTEXT_H
