
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
   
};
}


#endif //ISOUNDSYSTEM_H
