#ifndef WALLCOMPONENT_H
#define WALLCOMPONENT_H
#include "Direction.h"
#include "ObjectModel/Component.h"


class WallComponent final: public fovy::Component {
public:
    WallComponent(fovy::GameObject& pParent, fovy::Direction)
        : Component{pParent, "WallComponent"} {
    }

    void Update() override;
private:
    fovy::Direction m_wallDirection{fovy::Direction::None};
};



#endif //WALLCOMPONENT_H
