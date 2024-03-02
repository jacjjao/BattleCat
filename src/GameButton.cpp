#include "GameButton.hpp"
#include "Util/Input.hpp"

void GameButton::AddOnClickCallBack(const std::function<void()>& func) {
    m_OnClickCallBacks.push_back(func);
}

void GameButton::Update() {
    if (IsClicked()) {
        for (const auto &callback : m_OnClickCallBacks) {
            callback();
        }
    }
}

bool GameButton::IsClicked() {
    const auto size = GetScaledSize();
    const auto top_left_pos = GetTransform().translation - size / 2.0f;
    
    const auto pointInRect = [](const glm::vec2 rect_tl_pos, const glm::vec2 rect_size,
                                const glm::vec2 p) -> bool {
        bool outside = p.x < rect_tl_pos.x || p.x > rect_tl_pos.x + rect_size.x ||
                       p.y < rect_tl_pos.y || p.y > rect_tl_pos.y + rect_size.y;
        return !outside;
    };

    return Util::Input::IsKeyDown(Util::Keycode::MOUSE_LB) && pointInRect(top_left_pos, size, Util::Input::GetCursorPosition());
}
