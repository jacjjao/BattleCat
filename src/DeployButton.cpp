#include "DeployButton.hpp"
#include "Util/Root.hpp"

void DeployButton::Update(const double dt) {
    m_Timer.Update(dt);
    if (m_Timer.GetState() != Timer::State::START) {
        UpdateClickEvent();
    }
}

void DeployButton::SetCoolDownTime(const double time) {
    m_Timer.SetTimeOutDur(time);
}

void DeployButton::SetCost(const int cost) {
    n_Cost = cost;
}

void DeployButton::DrawCost() {
    if(!n_Cost){
        return;
    }
    Util::Transform t;
    t.translation = GetBottomRightPos();
    m_Cost.Display(n_Cost,glm::vec2(t.translation.x - 9,t.translation.y + 13),50.0f,18);
}

void DeployButton::StartCoolDown() {
    m_Timer.Start();
}
