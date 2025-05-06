
#ifndef ISOUNDSYSTEM_H
#define ISOUNDSYSTEM_H
#include <string>


namespace fovy{

class ISoundSystem
{
public:
    virtual ~ISoundSystem() = default;

    struct SoundClip
    {
        SoundClip() = default;
        virtual ~SoundClip() = default;

        virtual void Play(float volume, int loops) = 0;
        virtual void Pause() = 0;
        virtual void Resume() = 0;
        virtual void Stop() = 0;
    };

    virtual SoundClip& Load(const std::string& filePath) = 0;
    virtual void PlayAsync(const std::string& path, float volume, int loops) = 0;
    // virtual void Play(const std::filesystem::path& file, float volume, int loops = 0) = 0;
    // virtual void Pause(const std::filesystem::path& file) = 0;
    // virtual void PauseAll() = 0;
    // virtual void Resume(const std::filesystem::path& file) = 0;
    // virtual void ResumeAll() = 0;
    // virtual void Stop(const std::filesystem::path& file) = 0;
    // virtual void StopAll() = 0;
};
}


#endif //ISOUNDSYSTEM_H
