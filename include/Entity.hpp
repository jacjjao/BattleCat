#pragma once

#include <glm/vec2.hpp>

class Entity {
public:
    enum class State {
        WALK,
        ATTACK,
        KNOCK_BACK
    };

    virtual ~Entity() = default;

    virtual void Walk() = 0;

    virtual bool IsDead() const = 0;

protected:
    int m_Health = 0;
    int m_MoveSpeed = 0;
    int m_Atk = 0;
    int m_AtkPrepTime = 0;
    int m_AtkCoolDownTime = 0;
    int m_LastAtkTime = 0;
    State m_State = State::WALK;
    int m_PosX;
};
