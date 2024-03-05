#include "GameButton.hpp"
#include "Util/Input.hpp"
#include "Utility.hpp"
#include "Util/BGM.hpp"

GameButton::GameButton(const std::string &btn_path) {
    SetDrawable(std::make_shared<Util::Image>(btn_path));
}

GameButton::GameButton(const std::string &btn_path,
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
}

void GameButton::SetClickSound(const std::function<void()>& click_sound) {
    if (click_sound) {
        AddButtonEvent(click_sound);
    }
}

void GameButton::AddButtonEvent(const std::function<void()> &func) {
    m_ButtonEvents.push_back(func);
}

void GameButton::Update() {
    if(!m_HoverBorder){
        return;
    }

    if (IsMouseHovering()) {
        m_HoverBorder->SetVisible(true);
        m_HoverBorder->Play();
        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
            for (const auto &callback : m_ButtonEvents) {
                callback();
            }
        }
    }
    else{
        m_HoverBorder->SetVisible(false);
        m_HoverBorder->Pause();
    }
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
    m_HoverBorder->SetPosition(x, y);
    //m_text->SetPosition(x,y);
}

void GameButton::SetZIndex(const float index) {
    GameObjectEx::SetZIndex(index);
    m_HoverBorder->SetZIndex(index + 0.001f);
    //m_text->SetZIndex(index + 0.001f);
}

void GameButton::SetScale(float scale) {
    GameObjectEx::SetScale(scale,scale);
    m_HoverBorder->SetScale(scale,scale);
    //m_text->SetScale(m_text->GetScale().x * scale,m_text->GetScale().y * scale);
}

void GameButton::SetWidthScale(float scale){
    GameObjectEx::SetScale(scale, GameObjectEx::GetScale().y);
    m_HoverBorder->SetScale(scale, GameObjectEx::GetScale().y);
};

void GameButton::SetText(const std::string &txt_img_path,float scale){
    if (m_text) {
        RemoveChild(m_text);
    }
    auto text = std::make_shared<GameObjectEx>();
    text->SetDrawable(std::make_shared<Util::Image>(txt_img_path));
    text->SetScale(scale,scale);
    text->SetPosition(this->GetPosition());
    text->SetZIndex(this->GetZIndex() + 0.001f);
    m_text = std::move(text);
    AddChild(m_text);
};

bool GameButton::IsMouseHovering() const {
    const auto size = GetScaledSize();
    const auto top_left_pos = GetTransform().translation - size / 2.0f;
    return PointInRect(top_left_pos, size, Util::Input::GetCursorPosition());
}

/*std::shared_ptr<GameButton>
CreateGameYellowButton(const std::string &btn_path,
                       std::initializer_list<std::string> border_paths) {
    auto button = std::make_unique<GameButton>();
    button->SetDrawable(std::make_shared<Util::Image>(btn_path));

    auto border = std::make_shared<AnimatedGameObject>(border_paths);
    border->SetLooping(true);
    border->SetInterval(67);

    button->SetHoverBorder(border);

    return button;
}*/
