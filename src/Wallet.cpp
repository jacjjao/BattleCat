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
#define MAXDIGIT 5
#define MOST_RIGHT_X 590
    auto Cur = static_cast<int>(m_CurMoney);
    auto Max = static_cast<int>(m_MaxMoney);
    auto nextX = MOST_RIGHT_X;

    m_num->Draw(Util::Transform{.translation = glm::vec2(nextX-=30,300)},5.0f,11);
    for(int i=0;i<MAXDIGIT;i++){
        short int MaxNum = GetDigit(Max,i);
        if(MaxNum<10){
            m_num->Draw(Util::Transform{.translation=glm::vec2(nextX-=30,300)},5.0f,MaxNum);
        }
    }

    m_num->Draw(Util::Transform{.translation = glm::vec2(nextX-=30,300)},5.0f,10);
    for(int i=0;i<MAXDIGIT;i++){
        short int CurrNum = GetDigit(Cur,i);
        if(CurrNum<10){
            m_num->Draw(Util::Transform{.translation=glm::vec2(nextX-=30,300)},5.0f,CurrNum);
        }
    }
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

short int Wallet::GetDigit(int number, short int digit) {
    for(short i=0;i<digit;i++) {
        number/= 10;
        if(number==0) { return 10;}
    }
    return static_cast<short>(number%10);
};