#include "UnitCard.hpp"
#include "Util/Input.hpp"
#include "Utility.hpp"
#include "GameObjectEx.hpp"
#include "Util/Image.hpp"

void UnitCard::Dragging(){
    m_DragImg->SetPosition(Util::Input::GetCursorPosition());
    m_DragImg->Draw();
}

void UnitCard::Put_OFF() {
    return;
}

void UnitCard::Unpressed() {
    return;
}

bool UnitCard::IsMouseHovering(){
    const auto size = GetScaledSize();
    const auto top_left_pos = GetTransform().translation - size / 2.0f;
    return PointInRect(top_left_pos, size, Util::Input::GetCursorPosition());
}

