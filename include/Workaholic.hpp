#ifndef WORKAHOLIC_HPP
#define WORKAHOLIC_HPP

#include <array>
#include "GameButton.hpp"
#include "Wallet.hpp"
#include "NumberSystem.hpp"

class Workholic
{
public:
    Workholic(Wallet& wallet);

    bool CanLevelUp() const;

    void LevelUp();

    void Update();

    void Draw();

private:
    /*static constexpr std::array<int, 9> s_LevelSpend = {
        0, 0, 200, 400, 800, 1600, 3200, 5000, 6400
    };*/

    static constexpr std::array<int, 9> s_LevelSpend = {
        0, 0, 2, 4, 8, 16, 32, 50, 64
    };

    int m_Level = 1;

    GameButton m_Btn;

    std::shared_ptr<Util::Image> m_Img;
    AnimatedGameObject m_Anime;

    Util::Text m_LevelText;
    Util::Text m_SpendText;
    
    NumberSystem m_Spendnum = NumberSystem({RESOURCE_DIR"/scene/yellowbit/0.png",RESOURCE_DIR"/scene/yellowbit/1.png",RESOURCE_DIR"/scene/yellowbit/2.png",RESOURCE_DIR"/scene/yellowbit/3.png",RESOURCE_DIR"/scene/yellowbit/4.png",RESOURCE_DIR"/scene/yellowbit/5.png",
            RESOURCE_DIR"/scene/yellowbit/6.png",RESOURCE_DIR"/scene/yellowbit/7.png",RESOURCE_DIR"/scene/yellowbit/8.png",RESOURCE_DIR"/scene/yellowbit/9.png"});
    NumberSystem m_Levelnum = NumberSystem({RESOURCE_DIR"/scene/whitenumber/0.png",RESOURCE_DIR"/scene/whitenumber/1.png",RESOURCE_DIR"/scene/whitenumber/2.png",RESOURCE_DIR"/scene/whitenumber/3.png",RESOURCE_DIR"/scene/whitenumber/4.png",RESOURCE_DIR"/scene/whitenumber/5.png",
            RESOURCE_DIR"/scene/whitenumber/6.png",RESOURCE_DIR"/scene/whitenumber/7.png",RESOURCE_DIR"/scene/whitenumber/8.png",RESOURCE_DIR"/scene/whitenumber/9.png"});

    std::shared_ptr<Util::Image> m_dollar = std::make_shared<Util::Image>(RESOURCE_DIR"/scene/yellowbit/dollar.png");
    std::shared_ptr<Util::Image> m_LEVEL = std::make_shared<Util::Image>(RESOURCE_DIR"/scene/whitenumber/LEVEL.png");
    std::shared_ptr<Util::Image> m_MAX = std::make_shared<Util::Image>(RESOURCE_DIR"/scene/yellowbit/MAX.png");

    Wallet& m_Wallet;
};

#endif
