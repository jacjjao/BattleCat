#include "Wallet.hpp"
#include "Util/Text.hpp"

Wallet::Wallet(int level)
    : m_Text(RESOURCE_DIR "/font/Inter.ttf", 40, "HI", Util::Color(255, 255, 0, 255)) {
    assert(level < s_MoneyMax.size());
    m_MaxMoney = s_MoneyMax[level];
}

void Wallet::Update(float dt) {
    m_CurMoney = std::min(m_CurMoney + m_MoneyDelta * dt,
                          static_cast<float>(m_MaxMoney));
}

void Wallet::Draw() {
    std::array<char, 50> buf;
    snprintf(buf.data(), buf.size() - 1, "%d/%d$", static_cast<int>(m_CurMoney), m_MaxMoney);
    m_Text.SetText(buf.data());
    Util::Transform pos;
    pos.translation = glm::vec2(500, 300);
    m_Text.Draw(pos, 2.0f);
}

bool Wallet::CanDeploy(int required) {
    return static_cast<float>(required) <= m_CurMoney;
}

void Wallet::Spend(int money) {
    assert(CanDeploy(money));
    m_CurMoney -= static_cast<float>(money);
}
