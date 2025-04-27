#include "SoundLogSystem.h"

#include <iostream>

SoundLogSystem::SoundLogSystem(std::unique_ptr<ISoundSystem>&& soundSystem): m_soundSystem{std::move(soundSystem)} {
}

void SoundLogSystem::PlayAsync(const std::string& path, float volume, int loops) {
    std::cout << "Playing sound: " << path << " with volume: " << volume << " and loops: " << loops << std::endl;
    m_soundSystem->PlayAsync(path, volume, loops);
}
