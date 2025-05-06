#include "SDLSoundSystem.h"

#include <filesystem>
#include <iostream>
#include <mutex>
#include <queue>
#include <SDL_mixer.h>
#include <stdexcept>
#include <unordered_map>

#include "SDL.h"

namespace fovy {

    class SDLSoundSystem::SoundSystemImpl {
    public:
        SoundSystemImpl() {
            if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
                throw std::runtime_error("Failed to initialize SDL audio subsystem");
            }

            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                throw std::runtime_error("Failed to initialize SDL audio");
            }

            int flags = MIX_INIT_OGG | MIX_INIT_MP3;
            if ((Mix_Init(flags) & flags) != flags) {
                throw std::runtime_error("Failed to initialize SDL audio formats");
            }

            m_workerThread = std::thread([this]() { WorkerLoop(); });
        };

        void PlayAsync(const std::string& path, float volume, int loops) {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_playRequests.push({ path, volume, loops });
            }
            m_condition.notify_one();
        }

        void Pause() {
            std::lock_guard<std::mutex> lock(m_mutex);
            Mix_Pause(-1);
        }

        void Resume() {
            std::lock_guard<std::mutex> lock(m_mutex);
            Mix_Resume(-1);
        }

        ~SoundSystemImpl() {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_running = false;
            }
            m_condition.notify_one();
            if (m_workerThread.joinable()) {
                m_workerThread.join();
            }

            for (auto& sound : m_loadedEffects) {
                sound.second.reset();
            }
            for (auto& music : m_loadedMusic) {
                music.second.reset();
            }

            Mix_CloseAudio();
            Mix_Quit();
            SDL_QuitSubSystem(SDL_INIT_AUDIO);
        }

        SoundClip& Load(const std::string& filePath) {
            std::filesystem::path path{ filePath };
            if (!std::filesystem::exists(path)) {
                throw std::runtime_error("File not found: " + filePath);
            }
            std::string extension = path.extension().string();
            if (extension == ".wav") {
                return *m_loadedEffects.insert({ filePath, std::make_unique<SDLSound>(filePath) }).first->second;
            } else {
                return *m_loadedMusic.insert({ filePath, std::make_unique<SDLMusic>(filePath) }).first->second;
            }
        }
    private:
        struct PlayRequest {
            std::string filePath;
            float volume;
            int loops;
        };

        std::queue<PlayRequest> m_playRequests;
        std::mutex m_mutex;
        std::condition_variable m_condition;
        std::thread m_workerThread;
        bool m_running{ true };


        void WorkerLoop() {
            while (true) {
                PlayRequest request;
                {
                    std::unique_lock<std::mutex> lock(m_mutex);
                    m_condition.wait(lock, [this] { return !m_playRequests.empty() || !m_running; });
                    if (!m_running && m_playRequests.empty()) {
                        break;
                    }
                    std::cout << "Processing sound request" << std::endl;
                    request = std::move(m_playRequests.front());
                    m_playRequests.pop();
                }

                try {
                    std::filesystem::path path{ request.filePath };
                    if (!std::filesystem::exists(path)) {
                        throw std::runtime_error("File not found: " + request.filePath);
                    }

                    SoundClip* clip = nullptr;
                    std::string extension = path.extension().string();

                    {
                        std::lock_guard<std::mutex> lock(m_mutex);
                        if (extension == ".wav") {
                            auto it = m_loadedEffects.find(request.filePath);
                            if (it != m_loadedEffects.end()) {
                                clip = it->second.get();
                            } else {
                                auto sound = std::make_unique<SDLSound>(request.filePath);
                                clip = sound.get();
                                m_loadedEffects[request.filePath] = std::move(sound);
                            }
                        } else {
                            auto it = m_loadedMusic.find(request.filePath);
                            if (it != m_loadedMusic.end()) {
                                clip = it->second.get();
                            } else {
                                auto music = std::make_unique<SDLMusic>(request.filePath);
                                clip = music.get();
                                m_loadedMusic[request.filePath] = std::move(music);
                            }
                        }

                        if (clip) {
                            clip->Play(request.volume, request.loops);
                        }
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error playing sound: " << e.what() << std::endl;
                }
            }
        }

        struct SDLSound final: SoundClip {
        public:
            SDLSound(const std::string& path) {
                m_chunk = Mix_LoadWAV(path.c_str());
                if (!m_chunk) {
                    throw std::runtime_error("Failed to load sound: " + path);
                }
                m_channel = -1;
            }

            void Play(float volume, int loops) override {
                Mix_VolumeMusic(static_cast<int>(volume * MIX_MAX_VOLUME));
                m_channel = Mix_PlayChannel(-1, m_chunk, loops);
                if (m_channel == -1) {
                    throw std::runtime_error("Failed to play sound");
                }
            }

            void Pause() override {
                if (m_channel != -1) {
                    Mix_Pause(m_channel);
                }
            }
            void Resume() override {
                if (m_channel != -1) {
                    Mix_Resume(m_channel);
                }
            }
            void Stop() override {
                if (m_channel != -1) {
                    Mix_HaltChannel(m_channel);
                }
            }

        private:
            Mix_Chunk* m_chunk{nullptr};
            int m_channel;
        };

        struct SDLMusic final: public SoundClip {
        public:
            SDLMusic(const std::string& path) {
                m_chunk = Mix_LoadWAV(path.c_str());
                if (!m_chunk) {
                    throw std::runtime_error("Failed to load sound: " + path);
                }
                m_channel = -1;
            }

            void Play(float volume, int loops) override {
                Mix_VolumeMusic(static_cast<int>(volume * MIX_MAX_VOLUME));
                m_channel = Mix_PlayChannel(-1, m_chunk, loops);
                if (m_channel == -1) {
                    throw std::runtime_error("Failed to play sound");
                }
            }

            void Pause() override {
                if (Mix_PlayingMusic()) {
                    Mix_PausedMusic();
                }
            }
            void Resume() override {
                if (Mix_PausedMusic()) {
                    Mix_ResumeMusic();
                }
            }
            void Stop() override {
                if (Mix_PlayingMusic()) {
                    Mix_HaltMusic();
                }
            }

        private:
            Mix_Chunk* m_chunk{nullptr};
            int m_channel;
        };

        std::unordered_map<std::string, std::unique_ptr<SDLSound>> m_loadedEffects;
        std::unordered_map<std::string, std::unique_ptr<SDLMusic>> m_loadedMusic;
    };

    SDLSoundSystem::SDLSoundSystem() {
        m_impl = new SoundSystemImpl();
    }

    SDLSoundSystem::~SDLSoundSystem() {
        delete m_impl;
    }

    ISoundSystem::SoundClip& SDLSoundSystem::Load(const std::string& filePath) {
        return m_impl->Load(filePath);
    }

    void SDLSoundSystem::PlayAsync(const std::string& path, float volume, int loops) {
        return m_impl->PlayAsync(path, volume, loops);
    }
}
