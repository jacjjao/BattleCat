#include "Timer.hpp"

void Timer::SetTimeOutDur(const double time_out_dur) {
    m_TimeOutDur = time_out_dur;
}

void Timer::Start() {
    m_State = State::START;
    m_TimeRemained = m_TimeOutDur;
}

void Timer::SetTimeOutEvent(std::function<void()> callback) {
    m_Callback = callback;
}

void Timer::Update() {
    if (m_State == State::STOP) {
        return;
    }
    m_TimeRemained -= Util::Time::GetDeltaTime();
    if (m_TimeRemained <= 0.0) {
        m_State = State::STOP;
        m_Callback();
    }
}
