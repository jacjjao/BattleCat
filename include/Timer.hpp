#ifndef TIMER_HPP
#define TIMER_HPP

#pragma once

#include <functional>
#include "Util/Time.hpp"

class Timer {
public:
    enum class State {
        START,
        STOP
    };

    void SetTimeOutDur(double time_out_dur);

    void Start();

    void SetTimeOutEvent(std::function<void()> callback);

    void Update();

private:
    double m_TimeOutDur;
    double m_TimeRemained;
    std::function<void()> m_Callback;
    State m_State = State::STOP;
};
#endif
