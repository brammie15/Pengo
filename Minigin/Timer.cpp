#include "Timer.h"

#include <iostream>

double fovy::Time::FixedDeltaTime() const {
    return m_FixedDeltaTime;
}

double fovy::Time::DeltaTime() const {
    return m_DeltaTime;
}

std::chrono::nanoseconds fovy::Time::SleepDuration() const {
    constexpr auto msPerFrame = std::chrono::milliseconds(static_cast<int>(1000.f / FPS));
    const std::chrono::nanoseconds sleepTime = (m_PrevTime + msPerFrame - std::chrono::high_resolution_clock::now());

    return sleepTime;
}

void fovy::Time::Update() {
    const auto currentTime = std::chrono::high_resolution_clock::now();
    m_DeltaTime = std::chrono::duration<double>(currentTime - m_PrevTime).count();
    m_PrevTime = currentTime;
}
