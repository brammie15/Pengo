#include "AchievementManager.h"

#include <isteamuser.h>
#include <isteamuserstats.h>

namespace fovy {
    void AchievementManager::ScoreChanged(int newPoints) {
        if (newPoints >= 500 && !HasWinner) {
            HasWinner = true;
            UnlockAchievement();
        }
    }

    void AchievementManager::UnlockAchievement() {
        SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
        SteamUserStats()->StoreStats();
    }
} // fovy