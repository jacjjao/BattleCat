#ifndef WALLET_HPP
#define WALLET_HPP

#include <array>
#include "Util/Text.hpp"

class Wallet
{
public:
    Wallet(int level);

    void Update(float dt);

    void Draw();

    bool CanDeploy(int required);

    void Spend(int money);

    void SetWalletDelta(float delta);

private:
    static constexpr std::array<int, 20> s_MoneyMax = {
         500,  600,  700,  800,  900, 1000, 1100, 1200, 1300, 1400,
        1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400
    };

    float m_MoneyDelta = 30.0;
    float m_CurMoney = 0.0f;
    int m_MaxMoney = 0;

    Util::Text m_Text;
};

#endif