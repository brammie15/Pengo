#include "Timer.h"

double dae::Time::FixedDeltaTime() const {
    return m_FixedDeltaTime;
}

double dae::Time::DeltaTime() const {
    return m_DeltaTime;
}

std::chrono::nanoseconds dae::Time::SleepDuration() const {
    constexpr auto msPerFrame = std::chrono::milliseconds(static_cast<int>(1000.f / FPS));
    std::chrono::nanoseconds sleepTime = (m_PrevTime + msPerFrame - std::chrono::high_resolution_clock::now());

    return sleepTime;
}

void dae::Time::Update() {
    const auto currentTime = std::chrono::high_resolution_clock::now();
    m_DeltaTime = std::chrono::duration<double>(currentTime - m_PrevTime).count();
    m_PrevTime = currentTime;
}
