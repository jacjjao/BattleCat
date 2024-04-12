#include "Camera.hpp"
#include "Util/Input.hpp"
#include "Config.hpp"

void Camera::Move(const float dx) {
    m_Transform.translation.x += dx;
}

glm::vec2 Camera::UpdateMousePos() {
    const auto mouse_pos = Util::Input::GetCursorPosition();
    const auto delta = mouse_pos - m_LastMousePos;
    m_LastMousePos = mouse_pos;
    return delta;
}

void Camera::Update() {
    const auto mouse_delta = UpdateMousePos();
    if (Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB)) {
        Move(mouse_delta.x);
    }
    if (Util::Input::IsKeyDown(Util::Keycode::R)) {
        Reset();
    }
}

void Camera::Reset() {
    UpdateMousePos();
    m_Transform.scale = {1.0f, 1.0f};
    m_Transform.translation = {0.0f, 0.0f};
}

Util::Transform Camera::GetTransform() const {
    return m_Transform;
}

void Camera::SetTransform(const Util::Transform &t) {
    m_Transform = t;
}
