#ifndef BATTLECAT_DRAGGABLE_HPP
#define BATTLECAT_DRAGGABLE_HPP

enum class DragState : size_t{
    UNPRESSED = 0,
    PUT_OFF,
    DRAGGING
};

class Draggable{
public:
    virtual void Dragging() = 0;
    virtual bool IsMouseHovering() = 0;

    [[nodiscard]]
    DragState GetState() const {
        return m_State;
    };
protected:
    DragState m_State = DragState::UNPRESSED;
};


#endif // BATTLECAT_DRAGGABLE_HPP
