#include "StageList.hpp"
#include "Util/Input.hpp"
#include "config.hpp"
#include <cassert>

StageList::StageList() {
    m_Cards.reserve(STAGE_COUNT);
    for (int i = 0; i < STAGE_COUNT; ++i) {
        m_Cards.emplace_back("");
    }
    m_MinCursorX = -StageCard::GetImgSize().x / 2.0f - CARD_MARGIN;
    m_MaxCursorX = (StageCard::GetImgSize().x + CARD_MARGIN) *
                       static_cast<float>(m_Cards.size() - 1) +
                   StageCard::GetImgSize().x / 2.0f + CARD_MARGIN;
}

void StageList::Update() {
    const auto mp = Util::Input::GetCursorPosition();
    const auto dm = mp - m_PrevMouse;
    m_PrevMouse = mp;
    m_MouseDown = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);
    if (!m_MouseDown) {
        constexpr float speed_x = 5.0f;
        if (m_CursorX < 0.0f) {
            m_CursorX = std::min(m_CursorX + speed_x, 0.0f);
        }
        else if (m_CursorX > (StageCard::GetImgSize().x + CARD_MARGIN) *
                            static_cast<float>(m_Cards.size() - 1)) {
            m_CursorX = std::max(m_CursorX - speed_x,
                                 (StageCard::GetImgSize().x + CARD_MARGIN) *
                                     static_cast<float>(m_Cards.size() - 1));
        } else {
            const int cur_stage = std::round(
                m_CursorX / (StageCard::GetImgSize().x + CARD_MARGIN));
            const float target_x = (StageCard::GetImgSize().x + CARD_MARGIN) *
                                   static_cast<float>(cur_stage);
            if (m_CursorX > target_x) {
                m_CursorX = std::max(m_CursorX - speed_x, target_x);
            } else {
                m_CursorX = std::min(m_CursorX + speed_x, target_x);
            }
        }
        return;
    }
    m_CursorX -= dm.x;
    m_CursorX = std::clamp(m_CursorX, m_MinCursorX, m_MaxCursorX);
}

void StageList::Draw() {
    constexpr int half_win_width = static_cast<float>(WINDOW_WIDTH) / 2.0f;

    int cur_stage = std::round(m_CursorX / (StageCard::GetImgSize().x + CARD_MARGIN));
    if (cur_stage < 0) {
        cur_stage = 0;
    } else if (cur_stage >= m_Cards.size()) {
        cur_stage = m_Cards.size() - 1;
    }

    float dx =
        (StageCard::GetImgSize().x + CARD_MARGIN) * cur_stage - m_CursorX;
    if (std::abs(dx) <= StageCard::GetImgSize().x / 2.0f) {
        float scale = (1.0 - std::abs(dx) / (StageCard::GetImgSize().x / 2.0f)) * 0.5f + 1.0f;
        m_Cards[cur_stage].SetScale(scale, scale);
    }
    m_Cards[cur_stage].Draw(dx, 0);

    // previous
    {
        float x = dx;
        for (int i = cur_stage - 1; i >= 0; --i) {
            x -= m_Cards[i + 1].GetScaledSize().x / 2.0f;
            x -= CARD_MARGIN;
            x -= m_Cards[i].GetScaledSize().x / 2.0f;
            if (x + m_Cards[i].GetScaledSize().x / 2.0f < -half_win_width) {
                break;
            }
            m_Cards[i].Draw(x, 0);
        }
    }

    // post
    { 
        float x = dx;
        for (int i = cur_stage + 1; i < m_Cards.size(); ++i) {
            x += m_Cards[i - 1].GetScaledSize().x / 2.0f;
            x += CARD_MARGIN;
            x += m_Cards[i].GetScaledSize().x / 2.0f;
            if (x - m_Cards[i].GetScaledSize().x / 2.0f > half_win_width) {
                break;
            }
            m_Cards[i].Draw(x, 0);
        }
    }

    m_Cards[cur_stage].SetScale(1.0f, 1.0f);
    m_cur_stage = cur_stage;
}
