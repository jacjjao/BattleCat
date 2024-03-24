#ifndef BATTLECAT_DRAGGABLE_HPP
#define BATTLECAT_DRAGGABLE_HPP

#include "Util/Input.hpp"

enum class DragState : size_t{
    UNPRESSED = 0,
    PUT_OFF,
    DRAGGING
};

class Draggable{
public:
    void Drag(){
        UpdateState();
        switch (m_State) {
            case DragState::UNPRESSED:{
                Unpressed();
                break;
            }
            case DragState::PUT_OFF:{
                Put_OFF();
                break;
            }
            case DragState::DRAGGING:{
                Dragging();
                break;
            }
        }
    };

    [[nodiscard]]
    DragState GetCurrentState() const {
        return m_State;
    };
protected:
    virtual void Unpressed() = 0;
    virtual void Put_OFF() = 0;
    virtual void Dragging() = 0;

    virtual bool IsMouseHovering() = 0;
    DragState m_State = DragState::UNPRESSED;
private:
    void UpdateState(){
        //const auto key = Util::Keycode::MOUSE_LB;
        const auto key = Util::Keycode::SPACE;
        if(Util::Input::IsKeyUp(key) && m_State == DragState::DRAGGING){
            m_State = DragState::PUT_OFF;
            return;
        }
        if(!Util::Input::IsKeyPressed(key)){
            m_State = DragState::UNPRESSED;
        }
        if(Util::Input::IsKeyDown(key) && IsMouseHovering()){
            m_State = DragState::DRAGGING;
        }
    };
};


#endif // BATTLECAT_DRAGGABLE_HPP
