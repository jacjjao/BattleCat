#ifndef WALLET_HPP
#define WALLET_HPP

#include <array>
#include "Util/Text.hpp"
#include "AnimatedGameObject.hpp"
#include "NumberSystem.hpp"

class Wallet
{
public:
    Wallet(int level);

    void Update(float dt);

    void Draw();

    bool CanDeploy(int required) const;

    void Spend(int money);

    void SetWalletDelta(float delta);

private:
    static constexpr std::array<int, 9> s_MoneyMax = {
        0, 500,  1000,  2500,  5000, 8000,  10000,  12000, 16500
    };
    static constexpr std::array<float, 9> s_DeltaMoney = {
        0, 100, 150, 200, 250, 300, 350, 400, 500
    };

    float m_MoneyDelta = 50.0;
    float m_CurMoney = 0.0f;
    int m_MaxMoney = 0;

    NumberSystem m_wallet;
    std::shared_ptr<Util::Image> m_slash = std::make_shared<Util::Image>(RESOURCE_DIR"/scene/slash.png");
    std::shared_ptr<Util::Image> m_dollar = std::make_shared<Util::Image>(RESOURCE_DIR"/scene/dollar.png");

    Util::Text m_Text;
};

#endif