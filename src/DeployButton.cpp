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
    if (!m_Text) {
        m_Text.emplace(RESOURCE_DIR "/font/Inter.ttf", 16, " ",
                       Util::Color(0, 0, 200, 255));
    }
    std::string str = "$" + std::to_string(cost);
    m_Text->SetText(str);
    m_TextTransform.translation = GetScaledSize();
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
    if (!m_Text) {
        return;
    }
    Util::Transform t;
    t.translation = GetBottomRightPos();
    auto sz = m_Text->GetSize();
    // Hard-coded for now. Not sure if GetBottomRightPos is bugged or m_Text->GetSize is causing issues
    t.translation.x -= sz.x * 0.3f;
    t.translation.y += sz.y * 0.7f;
    m_Text->Draw(t, 50.0f);
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
