#ifndef BATTLECAT_DRAGGABLE_HPP
#define BATTLECAT_DRAGGABLE_HPP

#include "Util/Input.hpp"

class Draggable{
public:
    enum class State : size_t{
        UNPRESSED = 0,
        PUT_OFF,
        DRAGGING,
        PICKUP,
    };
    virtual ~Draggable() = default;
public:
    void Drag(){
        UpdateState();
        switch (m_State) {
            case State::UNPRESSED:{
                Unpressed();
                break;
            }
            case State::PUT_OFF:{
                Put_OFF();
                break;
            }
            case State::DRAGGING:{
                Dragging();
                break;
            }
            case State::PICKUP:{
                PickUp();
                break;
            }
        }
    };

    [[nodiscard]]
    State GetCurrentState() const {
        return m_State;
    };
protected:
    virtual void Unpressed(){};
    virtual void Put_OFF(){};
    virtual void Dragging(){};
    virtual void PickUp(){};

    virtual bool IsMouseHovering() = 0;
    State m_State = State::UNPRESSED;
private:
    void UpdateState(){
        //const auto key = Util::Keycode::MOUSE_LB;
        const auto key = Util::Keycode::SPACE;
        if(m_State == State::PICKUP){
            m_State = State::DRAGGING;
            return;
        }
        if(Util::Input::IsKeyUp(key) && m_State == State::DRAGGING){
            m_State = State::PUT_OFF;
            return;
        }
        if(!Util::Input::IsKeyPressed(key)){
            m_State = State::UNPRESSED;
        }
        if(Util::Input::IsKeyDown(key) && IsMouseHovering()){
            m_State = State::PICKUP;
        }
    };
};


#endif // BATTLECAT_DRAGGABLE_HPP
