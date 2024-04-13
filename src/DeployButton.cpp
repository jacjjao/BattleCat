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
    m_Text.emplace(RESOURCE_DIR "/font/Inter.ttf", 16, " ",
                   Util::Color(0, 0, 200, 255));
    std::string str = "$" + std::to_string(cost);
    m_Text->SetText(str);
    m_TextTransform.translation = GetScaledSize();
}

void DeployButton::DrawCost() {
    if (m_Text) {
        Util::Transform t;
        t.translation = GetBottomRightPos();
        auto sz = m_Text->GetSize();
        // Hard-coded for now. Not sure if GetBottomRightPos is bugged or m_Text->GetSize is causing issues
        t.translation.x -= sz.x * 0.3f;
        t.translation.y += sz.y * 0.7f;
        m_Text->Draw(t, 50.0f);
    }
}

void DeployButton::StartCoolDown() {
    m_Timer.Start();
}
