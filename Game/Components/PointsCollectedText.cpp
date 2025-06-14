#include "PointsCollectedText.h"

#include "Timer.h"
#include "ObjectModel/GameObject.h"
#include "ObjectModel/Transform.h"

pengo::PointsCollectedText::PointsCollectedText(fovy::GameObject& pParent): Component{pParent, "ScoreCollected"} {
}

void pengo::PointsCollectedText::Update() {
    //Slowly move up
    m_moveTimer += static_cast<float>(fovy::Time::GetInstance().DeltaTime());
    if (m_moveTimer >= m_moveDuration) {
        this->GetGameObject()->Destroy();
    }

    GetTransform().Move(
        glm::vec3(0, -100 * fovy::Time::GetInstance().DeltaTime(), 0)
    );
}
