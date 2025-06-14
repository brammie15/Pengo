#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H
#include <memory>

#include "ISoundSystem.h"

namespace fovy {
    class SDLSoundSystem final: public ISoundSystem {
    public:

        SDLSoundSystem();
        ~SDLSoundSystem() override;

        SDLSoundSystem(const SDLSoundSystem& other) = delete;
        SDLSoundSystem(SDLSoundSystem&& other) noexcept = delete;
        SDLSoundSystem& operator=(const SDLSoundSystem& other) = delete;
        SDLSoundSystem& operator=(SDLSoundSystem&& other) noexcept = delete;

        SoundClip& Load(const std::string& filePath) override;
        void PlayAsync(const std::string& path, float volume, int loops) override;

        void ToggleMute() override;

    private:
        class SoundSystemImpl;
        SoundSystemImpl* m_impl;
    };
}


#endif //SOUNDSYSTEM_H
