#include "Workaholic.hpp"
#include "config.hpp"
#include <cassert>

Workholic::Workholic(Wallet &wallet)
    : m_Wallet(wallet),
      m_Btn(RESOURCE_DIR "/img/Workholic0.png") ,
      m_Anime({RESOURCE_DIR "/img/Workholic1.png",
               RESOURCE_DIR "/img/Workholic2.png"}),
      m_LevelText(RESOURCE_DIR "/font/Inter.ttf", 24, " ",
                  Util::Color(0, 255, 255, 255)),
      m_SpendText(RESOURCE_DIR "/font/Inter.ttf", 24, " ",
                  Util::Color(0, 255, 255, 255)) {
    m_Anime.SetInterval(100);
    m_Anime.SetLooping(true);
    m_Anime.Play();

    m_Img = std::make_shared<Util::Image>(RESOURCE_DIR "/img/Workholic0.png");

    m_Btn.AddButtonEvent([this] {
        if (CanLevelUp()) {
            LevelUp();
        }
    });
    
    const int hw = WINDOW_WIDTH / 2;
    const int hh = WINDOW_HEIGHT / 2;
    const auto half_size = m_Btn.GetScaledSize() * 0.5f;
    m_Btn.SetPosition(-hw + half_size.x, -hh + half_size.y);

    m_Btn.SetZIndex(10.0f);

    m_LevelText.SetText("LEVEL 1");
    m_SpendText.SetText(std::to_string(s_LevelSpend[2]));
}

bool Workholic::CanLevelUp() const {
    const int next_level = m_Level + 1;
    return next_level < s_LevelSpend.size() &&
           m_Wallet.CanDeploy(s_LevelSpend[next_level]);
}

void Workholic::LevelUp() {
    ++m_Level;
    assert(m_Level < s_LevelSpend.size());
    m_Wallet.Spend(s_LevelSpend[m_Level]);
    m_Wallet.LevelUp(m_Level);

    if (m_Level >= 8) {
        m_LevelText.SetText("MAX!!!");
        m_SpendText.SetText("");
    } else {
        m_LevelText.SetText("LEVEL " + std::to_string(m_Level));
        m_SpendText.SetText(std::to_string(s_LevelSpend[m_Level + 1]));
    }
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
    transform.translation.y += m_Btn.GetScaledSize().y * 0.4f;
    transform.translation.x += 10;
    m_LevelText.Draw(transform, 11.0f);
    //m_Levelnum.Display(m_Level,glm::vec2(transform.translation.x, transform.translation.y),11.0f);

    transform.translation.y -= m_Btn.GetScaledSize().y * 0.8f;
    transform.translation.x -= 5;
    m_SpendText.Draw(transform, 11.0f);
}
