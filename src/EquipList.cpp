#include "EquipList.hpp"
#include "Util/Input.hpp"
#include "Utility.hpp"

EquipCard::EquipCard(unsigned int unitnum, const float zIndex, bool form) {
    const int UnitNumLength = 3;
    m_UnitNum = unitnum;
    std::stringstream uni1_img;
    uni1_img << RESOURCE_DIR"/cats/uni/f/" << "uni" << std::string(UnitNumLength - std::to_string(unitnum).length(), '0') << unitnum << "_f00.png";
    std::stringstream uni2_img;
    uni2_img << RESOURCE_DIR"/cats/uni/c/" << "uni" << std::string(UnitNumLength - std::to_string(unitnum).length(), '0') << unitnum << "_c00.png";
    m_uni1 = std::make_shared<GameObjectEx>(std::make_unique<Util::Image>(uni1_img.str()),zIndex);
    m_uni2 = std::make_shared<GameObjectEx>(std::make_unique<Util::Image>(uni2_img.str()),zIndex);

    m_form = form;
    m_curruni = (m_form? m_uni2:m_uni1);
}

void EquipCard::Transform() {
    //false -> 1st form , true -> 2nd form.
    m_curruni = !m_form ? m_uni2:m_uni1;
    m_form = !m_form;
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

void EquipCard::Put_DOWN() {
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



