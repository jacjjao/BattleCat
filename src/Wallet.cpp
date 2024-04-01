#include "Wallet.hpp"
#include "Util/Text.hpp"

Wallet::Wallet(const int level)
    : m_Text(RESOURCE_DIR "/font/Inter.ttf", 40, "HI", Util::Color(255, 255, 0, 255)) {
    assert(level < s_MoneyMax.size());
    m_MaxMoney = s_MoneyMax[level];
}

void Wallet::Update(const float dt) {
    m_CurMoney = std::min(m_CurMoney + m_MoneyDelta * dt,
                          static_cast<float>(m_MaxMoney));
}

void Wallet::Draw() {
    constexpr int MAX_DIGIT = 5;
    constexpr int MOST_RIGHT_X = 590;

    auto Cur = static_cast<int>(m_CurMoney);
    auto Max = static_cast<int>(m_MaxMoney);
    auto nextX = MOST_RIGHT_X;

    const auto getTransform = [&nextX]() -> Util::Transform { 
        nextX -= 30;
        Util::Transform t;
        t.translation = glm::vec2(nextX, 300);
        return t;
    };

    const auto drawDigits = [this, &getTransform](int num) -> void {
        assert(num >= 0);
        if (num == 0) {
            m_num->Draw(getTransform(), 5.0f, 0);
            return;
        }
        while (num > 0) {
            int digit = num % 10;
            m_num->Draw(getTransform(), 5.0f, digit);
            num /= 10;
        }
    };

    m_num->Draw(getTransform(), 5.0f, 11); // $
    drawDigits(Max);

    m_num->Draw(getTransform(), 5.0f, 10); // '/'
    drawDigits(Cur);
}

bool Wallet::CanDeploy(const int required) const {
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