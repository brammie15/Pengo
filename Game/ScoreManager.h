#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "Event.h"

class ScoreManager {
public:
    bool LoadFromFile(const std::string& filename);
    bool SaveToFile(const std::string& filename) const;

    void AddScore(const std::string& name, int score);
    void PrintScores() const;


    std::vector<std::pair<std::string, int>> GetTopScores(int count) const;
    fovy::Event<>& GetOnScoreChangedEvent() {
        return OnScoresUpdated;
    }

private:
    std::map<std::string, int> scores;

    fovy::Event<> OnScoresUpdated;
};
#endif //SCOREMANAGER_H
