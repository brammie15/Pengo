#ifndef SCORESCREENMANAGER_H
#define SCORESCREENMANAGER_H
#include <array>

#include "Event.h"
#include "ScoreManager.h"
#include "ObjectModel/Component.h"
#include "UI/LetterRoller.h"

namespace pengo {
class ScoreScreenManager: public fovy::Component, public fovy::EventListener {
public:
    ScoreScreenManager(fovy::GameObject& pParent, std::array<pengo::LetterRoller*, 3> letterRollers);

    void Start() override;
    void Update() override;

    void SubmitScore();
private:
    std::array<pengo::LetterRoller*, 3> m_letterRollers;
    std::string m_currentScore;
    ScoreManager m_manager;
};
}



#endif //SCORESCREENMANAGER_H
