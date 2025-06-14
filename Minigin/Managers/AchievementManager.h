#ifndef ACHIEVEMENTMANAGER_H
#define ACHIEVEMENTMANAGER_H
#include <Singleton.h>

#include "Event.h"

namespace fovy {

class AchievementManager final: public Singleton<AchievementManager>, public EventListener {
public:
    void ScoreChanged(int newPoints);
private:

    void UnlockAchievement();
    AchievementManager() = default;

    bool HasWinner{ false };

    friend class Singleton<AchievementManager>;
};

} // fovy

#endif //ACHIEVEMENTMANAGER_H
