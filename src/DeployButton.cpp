#include "DeployButton.hpp"

void DeployButton::Update(const double dt) {
    m_Timer.Update(dt);
    if (m_Timer.GetState() == Timer::State::START || !UpdateClickEvent()) {
        return;
    }
    m_Timer.Start();
}

void DeployButton::SetCoolDownTime(const double time) {
    m_Timer.SetTimeOutDur(time);
}