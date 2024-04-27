#include "GameButton.hpp"
#include "Util/Input.hpp"
#include "Utility.hpp"
#include "Util/BGM.hpp"

GameButton::GameButton(const std::string& btn_path) {
    SetDrawable(std::make_shared<Util::Image>(btn_path));

    SetZIndex(0.0f);
}

GameButton::GameButton(const std::string& btn_path,
                       std::initializer_list<std::string> border_paths){
    SetDrawable(std::make_shared<Util::Image>(btn_path));

    auto border = std::make_shared<AnimatedGameObject>(border_paths);
    border->SetLooping(true);
    border->SetInterval(67);
    SetHoverBorder(border);
    if (!s_ClickSound) {
        s_ClickSound = std::make_unique<Util::SFX>(RESOURCE_DIR "/sounds/click.mp3");
    }
    SetClickSound([]{s_ClickSound->Play();});

    SetZIndex(0.0f);
}

void GameButton::SetClickSound(const std::function<void()>& click_sound) {
    if (click_sound) {
        AddButtonEvent(click_sound);
    }
}

void GameButton::AddButtonEvent(const std::function<void()> &func) {
    m_ButtonEvents.push_back(func);
}

void GameButton::Update(double) {
    UpdateClickEvent();
    UpdateHoverBorder();
}

void GameButton::SetHoverBorder(std::shared_ptr<AnimatedGameObject> border) {
    if (m_HoverBorder) {
        RemoveChild(m_HoverBorder);
    }
    m_HoverBorder = std::move(border);
    AddChild(m_HoverBorder);
}

void GameButton::SetPosition(const float x, const float y) {
    GameObjectEx::SetPosition(x, y);
    if (m_HoverBorder) {
        m_HoverBorder->SetPosition(x, y);
    }
}

void GameButton::SetZIndex(const float index) {
    GameObjectEx::SetZIndex(index);
    if (m_HoverBorder) {
        m_HoverBorder->SetZIndex(index + 0.001f);
    }
}

bool GameButton::UpdateClickEvent() {
    if (!IsMouseHovering() ||
        !Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
        return false;
    }
    for (const auto &callback : m_ButtonEvents) {
        callback();
    }
    return true;
}

void GameButton::UpdateHoverBorder() {
    if (!m_HoverBorder) {
        return;
    }
    if (IsMouseHovering()) {
        m_HoverBorder->SetVisible(true);
        m_HoverBorder->Play();
    } else {
        m_HoverBorder->SetVisible(false);
        m_HoverBorder->Pause();
    }
}

void GameButton::SetScale(float scale) {
    GameObjectEx::SetScale(scale,scale);
    if (m_HoverBorder) {
        m_HoverBorder->SetScale(scale, scale);
    }
}

bool GameButton::IsMouseHovering() const {
    const auto size = GetScaledSize();
    const auto top_left_pos = GetTransform().translation - size / 2.0f;
    return PointInRect(top_left_pos, size, Util::Input::GetCursorPosition());
}

