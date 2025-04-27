#ifndef SOUNDLOGSYSTEM_H
#define SOUNDLOGSYSTEM_H
#include <memory>

#include "ISoundSystem.h"


class SoundLogSystem: public fovy::ISoundSystem {
public:
    SoundLogSystem(std::unique_ptr<ISoundSystem>&& soundSystem);

    void PlayAsync(const std::string& path, float volume, int loops) override;
private:

    std::unique_ptr<ISoundSystem> m_soundSystem;
};



#endif //SOUNDLOGSYSTEM_H
