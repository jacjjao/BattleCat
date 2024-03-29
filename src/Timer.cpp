#include "Timer.hpp"

void Timer::SetTimeOutDur(const double time_out_dur) {
    m_TimeOutDur = time_out_dur;
}

void Timer::Start() {
    m_State = State::START;
    m_TimeRemained = m_TimeOutDur;
    m_TimeOut = false;
}

void Timer::Update(const double dt) {
    if (m_State == State::STOP) {
        return;
    }
    m_TimeRemained -= dt;
    if (m_TimeRemained <= 0.0) {
        m_State = State::STOP;
        m_TimeOut = true;
    }
}

bool Timer::IsTimeOut() {
    bool b = m_TimeOut;
    m_TimeOut = false;
    return b;
}

Timer::State Timer::GetState() const {
    return m_State;
}
