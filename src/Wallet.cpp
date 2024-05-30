#include "Wallet.hpp"
#include "Util/Text.hpp"
#include "Util/Input.hpp"
#include "Util/Time.hpp"

Wallet::Wallet(const int level)
    : m_Text(RESOURCE_DIR "/font/Inter.ttf", 40, "HI", Util::Color(255, 255, 0, 255)) {
    assert(level < int(s_MoneyMax.size()));
    m_MaxMoney = s_MoneyMax[level];
    m_MoneyDelta = s_DeltaMoney[level];
}

void Wallet::Update(const float dt) {
    constexpr double time_reload = 0.1;
    static double t = time_reload;

    if (t > 0.0) {
        t -= Util::Time::GetDeltaTime();
    }
    if (Util::Input::IsKeyPressed(Util::Keycode::SPACE) && t <= 0.0) {
        m_InfMoney = !m_InfMoney;
        t = time_reload;
    }

    m_CurMoney = std::min(m_CurMoney + m_MoneyDelta * dt,static_cast<float>(m_MaxMoney));
}

void Wallet::Draw() {
    Util::Transform tmp;
    tmp.translation = glm::vec2(590, 300);
    m_dollar->Draw(tmp, 5.0f);
    glm::vec2 rightmost_pos = NumberSystem::Display(m_MaxMoney,glm::vec2(560,300),5.0f,30,NumberSystem::YellowNumber);
    tmp.translation = glm::vec2(rightmost_pos.x - 30, 300);
    m_slash->Draw(tmp, 5.0f);
    if (m_InfMoney) {
        NumberSystem::Display(static_cast<int>(999999999),
                              glm::vec2(rightmost_pos.x - 60, 300), 5.0f, 30,
                              NumberSystem::YellowNumber);
    } else {
        NumberSystem::Display(static_cast<int>(m_CurMoney),
                              glm::vec2(rightmost_pos.x - 60, 300), 5.0f, 30,
                              NumberSystem::YellowNumber);
    }
}

bool Wallet::CanDeploy(const int required) const {
    return m_InfMoney || static_cast<float>(required) <= m_CurMoney;
}

void Wallet::Spend(const int money) {
    assert(CanDeploy(money));
    if (!m_InfMoney) {
        m_CurMoney -= static_cast<float>(money);
    }
}

void Wallet::SetWalletDelta(const float delta) {
    assert(delta > 0.0f);
    m_MoneyDelta = delta;
}

void Wallet::LevelUp(const int level) {
    m_MaxMoney = s_MoneyMax[level];
    m_MoneyDelta = s_DeltaMoney[level];
}

