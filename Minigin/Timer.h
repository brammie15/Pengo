#ifndef TIME_H
#define TIME_H
#include <chrono>
#include <Singleton.h>

namespace dae {
    class Time final : public Singleton<Time> {
    public:

        Time(const Time& other) = delete;
        Time(Time&& other) = delete;
        Time& operator=(const Time& other) = delete;
        Time& operator=(Time&& other) = delete;

        [[nodiscard]] double FixedDeltaTime() const;
        [[nodiscard]] double DeltaTime() const;

        [[nodiscard]] std::chrono::nanoseconds SleepDuration() const;

        void Update();
    private:
        friend class Singleton<Time>;

        Time() = default;
        static constexpr double m_FixedDeltaTime{ 1.0 / 60.0 };
        static constexpr double FPS{ 60 };
        double m_DeltaTime{};
        std::chrono::high_resolution_clock::time_point m_PrevTime;
    };
}

#endif //TIME_H
