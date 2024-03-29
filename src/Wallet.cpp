#include "Wallet.hpp"
#include "Util/Text.hpp"

Wallet::Wallet(const int level)
    : m_Text(RESOURCE_DIR "/font/Inter.ttf", 40, "HI", Util::Color(255, 255, 0, 255)) {
    assert(level < s_MoneyMax.size());
    m_MaxMoney = s_MoneyMax[level];
}

void Wallet::Update(const float dt) {
    m_CurMoney = std::min(m_CurMoney + m_MoneyDelta * dt,static_cast<float>(m_MaxMoney));
}

void Wallet::Draw() {
    m_dollar->Draw(Util::Transform{.translation=glm::vec2(590,300)},5.0f);
    glm::vec2 leftmost_pos = m_wallet.Display(m_MaxMoney,glm::vec2(560,300),5.0f);
    m_slash->Draw(Util::Transform{.translation=glm::vec2(leftmost_pos.x-30,300)},5.0f);
    m_wallet.Display(static_cast<int>(m_CurMoney),glm::vec2(leftmost_pos.x-60,300),5.0f);
}

bool Wallet::CanDeploy(const int required) {
    return static_cast<float>(required) <= m_CurMoney;
}

void Wallet::Spend(const int money) {
    assert(CanDeploy(money));
    m_CurMoney -= static_cast<float>(money);
}

void Wallet::SetWalletDelta(const float delta) {
    assert(delta > 0.0f);
    m_MoneyDelta = delta;
}

