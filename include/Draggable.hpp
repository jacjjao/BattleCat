#ifndef BATTLECAT_DRAGGABLE_HPP
#define BATTLECAT_DRAGGABLE_HPP

#include "Util/Input.hpp"
#include "Utility.hpp"

class Draggable{
public:
    enum class DragState : size_t{
        UNPRESSED = 0,
        PUT_DOWN,
        DRAGGING,
        PICKUP,
    };
    virtual ~Draggable() = default;
public:
    void Drag(){
        UpdateState();
        switch (m_DragState) {
            case DragState::UNPRESSED:{
                Unpressed();
                break;
            }
            case DragState::PUT_DOWN:{
                Put_DOWN();
                break;
            }
            case DragState::DRAGGING:{
                Dragging();
                break;
            }
            case DragState::PICKUP:{
                PickUp();
                break;
            }
        }
    };

    [[nodiscard]]
    DragState GetCurrentState() const {
        return m_DragState;
    };
protected:
    virtual void Unpressed(){};
    virtual void Put_DOWN(){};
    virtual void Dragging(){};
    virtual void PickUp(){};

    virtual bool IsMouseHovering() = 0;
    DragState m_DragState = DragState::UNPRESSED;
private:
    void UpdateState(){
        const auto key = Util::Keycode::MOUSE_LB;
        //const auto key = Util::Keycode::SPACE;
        if(m_DragState == DragState::PICKUP){
            m_DragState = DragState::DRAGGING;
            return;
        }
        if(Patch::MouseLBUP() && m_DragState == DragState::DRAGGING){
            m_DragState = DragState::PUT_DOWN;
            return;
        }
        if(!Util::Input::IsKeyPressed(key)){
            m_DragState = DragState::UNPRESSED;
            return;
        }
        if(Util::Input::IsKeyDown(key) && IsMouseHovering()){
            m_DragState = DragState::PICKUP;
            return;
        }
    };
};


#endif // BATTLECAT_DRAGGABLE_HPP
