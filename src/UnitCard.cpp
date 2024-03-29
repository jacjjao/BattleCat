#include "UnitCard.hpp"
#include "Util/Input.hpp"
#include "Utility.hpp"
#include "GameObjectEx.hpp"
#include "Util/Image.hpp"

void UnitCard::Dragging() {
    m_DragTrans.translation = Util::Input::GetCursorPosition();
    if (m_FrameTimer > 0) {
        m_DragTrans.scale += glm::vec2(0.1f,0.1f)*glm::vec2(m_minify ? -1:1);
        m_FrameTimer--;
    }
    m_Drawable->Draw(m_DragTrans,m_ZIndex+0.3f);
    SetVisible(OriginalVisible);
}

void UnitCard::Put_OFF() {
    m_minify = false;
    SetVisible(true);
    m_FrameTimer = 0;
    m_DragTrans.scale = glm::vec2(1.4f, 1.4f);
}

void UnitCard::Unpressed() {
    return;
}

void UnitCard::MinifyAnime(){
    if(m_minify || m_State == DragState::UNPRESSED){
        return;
    }
    m_DragTrans.scale = glm::vec2(1.35f,1.35f);
    m_FrameTimer = 10;
    m_minify = true;
}

void UnitCard::AmplifyAnime(){
    if(!m_minify || m_State == DragState::UNPRESSED){
        return;
    }
    m_DragTrans.scale = glm::vec2(0.35f,0.35f);
    m_FrameTimer = 10;
    m_minify = false;
}

bool UnitCard::IsMouseHovering(){
    const auto size = GetScaledSize();
    const auto top_left_pos = GetTransform().translation - size / 2.0f;
    return PointInRect(top_left_pos, size, Util::Input::GetCursorPosition());
}

