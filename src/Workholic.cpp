#include "Workaholic.hpp"
#include "config.hpp"
#include <cassert>
#include "Sound.hpp"

Workholic::Workholic(Wallet &wallet)
    : m_Btn(RESOURCE_DIR "/img/Workholic0.png"),
      m_Anime({RESOURCE_DIR "/img/Workholic1.png",RESOURCE_DIR "/img/Workholic2.png"}) ,
      m_Wallet(wallet){

    m_Anime.SetInterval(100);
    m_Anime.SetLooping(true);
    m_Anime.Play();

    m_Img = std::make_shared<Util::Image>(RESOURCE_DIR "/img/Workholic0.png");

    m_Btn.AddButtonEvent([this] {
        if (CanLevelUp()) {
            LevelUp();
            Sounds::Deploy->Play();
        }
        else{
            Sounds::Scrolling->Play();
        }
    });
    
    const int hw = WINDOW_WIDTH / 2;
    const int hh = WINDOW_HEIGHT / 2;
    const auto half_size = m_Btn.GetScaledSize() * 0.5f;
    m_Btn.SetPosition(-hw + half_size.x, -hh + half_size.y);
    m_Btn.SetZIndex(10.0f);

}

bool Workholic::CanLevelUp() const {
    const int next_level = m_Level + 1;
    return next_level < int(s_LevelSpend.size()) &&
           m_Wallet.CanDeploy(s_LevelSpend[next_level]);
}

void Workholic::LevelUp() {
    ++m_Level;
    assert(m_Level < int(s_LevelSpend.size()));
    m_Wallet.Spend(s_LevelSpend[m_Level]);
    m_Wallet.LevelUp(m_Level);
}

void Workholic::Update() {
    m_Btn.Update();
    if (CanLevelUp()) {
        m_Btn.SetDrawable(m_Anime.GetDrawable());
    } else {
        m_Btn.SetDrawable(m_Img);
    }
}

void Workholic::Draw() {
    m_Btn.Draw();

    Util::Transform transform = m_Btn.GetTransform();
    transform.translation.y += m_Btn.GetScaledSize().y * 0.4f + 5;
    transform.translation.x -= 7;
    m_LEVEL->Draw(transform,11.0f);
    m_Levelnum.Display(m_Level,glm::vec2(transform.translation.x+50, transform.translation.y),11.0f,20);

    transform.translation.y -= m_Btn.GetScaledSize().y * 0.8f;
    transform.translation.x += 20;

    if (m_Level >= 8) {
        transform.translation.x -= 10;
        m_MAX->Draw(transform,11.0f);
    }
    else{
        m_dollar->Draw(transform,11.0f);
        m_Spendnum.Display(
            s_LevelSpend[m_Level + 1],
            glm::vec2(transform.translation.x - 18, transform.translation.y),
            11.0f, 18);
    }
}
