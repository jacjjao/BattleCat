#include "UnitCard.hpp"
#include "Util/Input.hpp"
#include "Utility.hpp"
#include "GameObjectEx.hpp"
#include "Util/Image.hpp"

bool UnitCard::IsMouseHovering() const {
    const auto size = GetScaledSize();
    const auto top_left_pos = GetTransform().translation - size / 2.0f;
    return PointInRect(top_left_pos, size, Util::Input::GetCursorPosition());
}

void UnitCard::Dragging(){
    const auto key = Util::Keycode::SPACE;
    if(Util::Input::IsKeyUp(key) && m_State == UnitCardState::DRAGGING){
        m_State = UnitCardState::PUT_OFF;
        return;
    }
    if(!Util::Input::IsKeyPressed(key)){
        m_State = UnitCardState::UNPRESSED;
    }
    if(Util::Input::IsKeyDown(key) && IsMouseHovering()){
        m_State = UnitCardState::DRAGGING;
    }
    if(m_State == UnitCardState::DRAGGING){
        m_DragImg->SetVisible(true);
        m_DragImg->SetPosition(Util::Input::GetCursorPosition());
        m_DragImg->Draw();
    }
}

UnitCardState UnitCard::GetState() const{
    return m_State;
}

