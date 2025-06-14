#include "PengoSlider.h"

#include <ext/quaternion_common.hpp>

#include "Timer.h"
#include "ObjectModel/GameObject.h"

pengo::PengoSlider::PengoSlider(fovy::GameObject& pParent):
fovy::Component{pParent, "PengoSlider"} {
}

void pengo::PengoSlider::Update() {
    t += static_cast<float>(speed * fovy::Time::GetInstance().DeltaTime());
    if (t > 1.f) {
        t = 0.f; // Reset t to loop the slider
    }
    const auto newPos = glm::mix(startPos, endPos, t);
    GetGameObject()->GetTransform().SetLocalPosition(newPos);
}
