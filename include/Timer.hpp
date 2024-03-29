#ifndef TIMER_HPP
#define TIMER_HPP

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

    void Update(double dt);

    bool IsTimeOut();

    State GetState() const;

private:
    double m_TimeOutDur;
    double m_TimeRemained;
    bool m_TimeOut = false;
    State m_State = State::STOP;
};
#endif
