#include "AchievementManager.h"

#include <iostream>

namespace fovy {
    void AchievementManager::ScoreChanged(int newPoints) {
        if (newPoints >= 500 && !HasWinner) {
            HasWinner = true;
            UnlockAchievement();
        }
    }

    void AchievementManager::UnlockAchievement() {
        std::cout << "Unlocked Achievement" << std::endl;
    }
} // fovy