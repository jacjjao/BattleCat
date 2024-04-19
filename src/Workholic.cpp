#include "Workaholic.hpp"
#include "config.hpp"
#include <cassert>

Workholic::Workholic(Wallet &wallet)
    : m_Wallet(wallet),
      m_Btn(RESOURCE_DIR "/img/Workholic0.png",
            {RESOURCE_DIR "/img/Workholic1.png",
             RESOURCE_DIR "/img/Workholic2.png"}) {
    m_Btn.AddButtonEvent([this] {
        if (CanLevelUp()) {
            LevelUp();
        }
    });
    
    const int hw = WINDOW_WIDTH / 2;
    const int hh = WINDOW_HEIGHT / 2;
    const auto half_size = m_Btn.GetScaledSize() * 0.5f;
    m_Btn.SetPosition(-hw + half_size.x, -hh + half_size.y);

    m_Btn.SetZIndex(100.0f);
}

bool Workholic::CanLevelUp() const {
    const int next_level = m_Level + 1;
    const int next_level_spend = s_LevelSpend[m_Level + 1];
    return next_level < s_LevelSpend.size() &&
           m_Wallet.CanDeploy(next_level_spend);
}

void Workholic::LevelUp() {
    ++m_Level;
    assert(m_Level < s_LevelSpend.size());
    m_Wallet.Spend(s_LevelSpend[m_Level]);
    m_Wallet.LevelUp(m_Level);
}

void Workholic::Update() {
    m_Btn.Update();
    /* TODO: flashing */
}

void Workholic::Draw() {
    m_Btn.Draw();
}
