#include "EquipList.hpp"
#include "Util/Input.hpp"
#include "Utility.hpp"

void EquipCard::Setform(const unsigned short form) {
    if(form != 1 && form != 2){
        throw std::invalid_argument("Form error , please input 1 or 2");
    }
    m_curruni = (form == 1 ? m_uni1:m_uni2);
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



