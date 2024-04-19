#include "EquipList.hpp"
#include "Util/Input.hpp"
#include "Utility.hpp"

void EquipCard::Transform() {
    //false -> 1 form , true -> 2 form.
    m_form = !m_form;
    m_curruni = (m_form? m_uni2:m_uni1);
}

void EquipCard::SetPos(float x, float y) {
    m_uni1->SetPosition(x,y);
    m_uni2->SetPosition(x,y);
}

void EquipCard::Dragging() {
    m_DragTrans.translation = Util::Input::GetCursorPosition();
    if (m_FrameTimer > 0) {
        m_DragTrans.scale += glm::vec2(0.05f,0.05f);
        m_FrameTimer--;
    }
    m_curruni->Draw(m_DragTrans,m_curruni->GetZIndex());
}

void EquipCard::Put_OFF() {
    m_FrameTimer = 0;
    m_DragTrans.scale = glm::vec2(1.0f, 1.0f);
}

void EquipCard::Unpressed() {
    m_curruni->Draw();
}

void EquipCard::PickUp() {
    m_FrameTimer = 10;
}

bool EquipCard::IsMouseHovering(){
    const auto size = m_curruni->GetScaledSize();
    const auto top_left_pos = m_curruni->GetPosition() - size / 2.0f;
    return PointInRect(top_left_pos, size, Util::Input::GetCursorPosition());
}



