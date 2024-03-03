#include "GameButton.hpp"
#include "Util/Input.hpp"
#include "Utility.hpp"
#include "Util/BGM.hpp"

GameButton::GameButton() {
    if (!s_ClickSound) {
        s_ClickSound = std::make_unique<Util::SFX>(RESOURCE_DIR "/sounds/click.mp3");
    }

    AddOnClickCallBack([] { 
        s_ClickSound->Play(); 
    });
}

GameButton::GameButton(const std::function<void()>& click_sound) {
    if (click_sound) {
        AddOnClickCallBack(click_sound);
    }
}

void GameButton::AddOnClickCallBack(const std::function<void()> &func) {
    m_OnClickCallBacks.push_back(func);
}

void GameButton::Update() {
    if(!m_HoverBorder){
        return;
    }

    if (IsMouseHovering()) {
        m_HoverBorder->SetVisible(true);
        m_HoverBorder->Play();
        if (Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB)) {
            for (const auto &callback : m_OnClickCallBacks) {
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
}

void GameButton::SetZIndex(const float index) {
    GameObjectEx::SetZIndex(index);
    m_HoverBorder->SetZIndex(index + 0.001f);
}

bool GameButton::IsMouseHovering() const {
    const auto size = GetScaledSize();
    const auto top_left_pos = GetTransform().translation - size / 2.0f;
    return PointInRect(top_left_pos, size, Util::Input::GetCursorPosition());
}

std::shared_ptr<GameButton>
CreateGameYellowButton(const std::string &btn_path,
                       std::initializer_list<std::string> border_paths) {
    auto button = std::make_unique<GameButton>();
    button->SetDrawable(std::make_shared<Util::Image>(btn_path));

    auto border = std::make_shared<AnimatedGameObject>(border_paths);
    border->SetLooping(true);
    border->SetInterval(67);


    const auto button_size = button->GetScaledSize();
    const auto border_size = border->GetScaledSize();
    border->SetScale(button_size.x / border_size.x,
                     button_size.y / border_size.y);


    button->SetHoverBorder(border);

    return button;
}