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
    static constexpr std::array<int, 9> s_LevelSpend = {
        0, 0, 200, 400, 800, 1600, 3200, 5000, 6400
    };

    int m_Level = 1;

    GameButton m_Btn;

    std::shared_ptr<Util::Image> m_Img;
    AnimatedGameObject m_Anime;

    Util::Text m_LevelText;
    Util::Text m_SpendText;

    NumberSystem m_Levelnum;
    NumberSystem m_Spendnum;

    Wallet& m_Wallet;
};

#endif