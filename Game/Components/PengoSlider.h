#ifndef PENGOSLIDER_H
#define PENGOSLIDER_H
#include <vec3.hpp>

#include "ObjectModel/Component.h"


namespace pengo {
    class PengoSlider: public fovy::Component {
    public:
        PengoSlider(fovy::GameObject& pParent);

        void Update() override;

    private:

        //(224.f * 3.f, 288.f * 3.f
        glm::vec3 startPos {-100.f, 400.f, 0.f};
        glm::vec3 endPos {288 * 4.f, 400.f, 0.f};

        float t {0.f};
        float speed {0.05f}; // Adjust speed as needed

    };
}


#endif //PENGOSLIDER_H
