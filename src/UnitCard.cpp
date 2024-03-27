#include "UnitCard.hpp"
#include "Util/Input.hpp"
#include "Utility.hpp"
#include "GameObjectEx.hpp"
#include "Util/Image.hpp"

void UnitCard::Dragging(){
    m_DragImg->SetPosition(Util::Input::GetCursorPosition());
    m_DragImg->Draw();
    SetVisible(OriginalVisible);
}

void UnitCard::Put_OFF() {
    SetVisible(true);
    return;
}

void UnitCard::Unpressed() {
    return;
}

void UnitCard::SetDragImgScale(float x,float y){
    m_DragImg->SetScale(x,y);
}

bool UnitCard::IsMouseHovering(){
    const auto size = GetScaledSize();
    const auto top_left_pos = GetTransform().translation - size / 2.0f;
    return PointInRect(top_left_pos, size, Util::Input::GetCursorPosition());
}

