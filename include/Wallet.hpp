#ifndef WALLET_HPP
#define WALLET_HPP

#include <array>
#include "Util/Text.hpp"
#include "AnimatedGameObject.hpp"

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
    static constexpr std::array<int, 21> s_MoneyMax = {
        0,    500,  600,  700,  800,  900,  1000, 1100, 1200, 1300, 1400,
        1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400};

    float m_MoneyDelta = 50.0;
    float m_CurMoney = 0.0f;
    int m_MaxMoney = 0;

    Util::Text m_Text;

    //-------------------
    std::shared_ptr<AnimatedGameObject> m_num = std::make_shared<AnimatedGameObject>
        (std::initializer_list<std::string>{RESOURCE_DIR"/scene/0.png",RESOURCE_DIR"/scene/1.png",RESOURCE_DIR"/scene/2.png",RESOURCE_DIR"/scene/3.png",RESOURCE_DIR"/scene/4.png",RESOURCE_DIR"/scene/5.png",RESOURCE_DIR"/scene/6.png",RESOURCE_DIR"/scene/7.png",RESOURCE_DIR"/scene/8.png",RESOURCE_DIR"/scene/9.png",RESOURCE_DIR"/scene/slash.png",RESOURCE_DIR"/scene/dollar.png"});
};

#endif