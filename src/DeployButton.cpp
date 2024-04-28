#include "DeployButton.hpp"
#include "Util/Root.hpp"

void DeployButton::Update(const double dt) {
    m_Timer.Update(dt);
    bool CoolDown = (m_Timer.GetState() == Timer::State::START);
    UpdateClickEvent(!CoolDown);
}

void DeployButton::SetCoolDownTime(const double time) {
    m_Timer.SetTimeOutDur(time);
}

void DeployButton::SetCost(const int cost) {
    n_Cost = cost;
}

void DeployButton::Draw() {
    GameObject::Draw(); // Yes. Bad code. I know
    DrawCost();

    // draw cool down bar if the button is in cool down state
    if (m_Timer.GetState() == Timer::State::STOP) {
        return;
    }
    constexpr float margin_x = 20;
    constexpr float margin_y = 15;
    constexpr float h = 10;
    const float w = (GetScaledSize().x - margin_x) * (1.0 - m_Timer.GetProcess());
    glm::vec2 pos = GetPosition();
    pos.x = pos.x + w / 2.0f - GetScaledSize().x / 2.0f + margin_x / 2.0;
    pos.y = pos.y - GetScaledSize().y / 2.0 + margin_y;
    s_Renderer->DrawRect(pos.x, pos.y, w, h, {0, 1, 1, 1});
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

void DeployButton::Init() {
    s_Renderer.emplace();
}

void DeployButton::DrawStates() {
    s_Renderer->DrawAll();
}
