#include "ScoreManager.h"

#include <fstream>
#include <iostream>
#include <sstream>

bool ScoreManager::LoadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    scores.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name;
        int score;
        if (std::getline(iss, name, '-') && (iss >> score)) {
            // Trim whitespace
            name.erase(name.find_last_not_of(" \t") + 1);
            name.erase(0, name.find_first_not_of(" \t"));
            scores[name] = score;
        }
    }

    return true;
}

bool ScoreManager::SaveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    for (const auto& pair : scores) {
        file << pair.first << " - " << pair.second << "\n";
    }

    return true;
}

void ScoreManager::AddScore(const std::string& name, int score) {
    scores[name] = score;

    OnScoresUpdated.Invoke();
}

void ScoreManager::PrintScores() const {
    for (const auto& [name, score] : scores) {
        std::cout << name << " - " << score << "\n";
    }
}

std::vector<std::pair<std::string, int>> ScoreManager::GetTopScores(int count) const {
    std::vector<std::pair<std::string, int>> topScores;
    for (const auto& [name, score] : scores) {
        topScores.emplace_back(name, score);
    }
    std::sort(topScores.begin(), topScores.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    if (topScores.size() > count) {
        topScores.resize(count);
    }
    return topScores;
}
