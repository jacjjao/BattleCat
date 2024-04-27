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

    void Stop();

    void Update(double dt);

    bool IsTimeOut();

    [[nodiscard]]
    State GetState() const;

private:
    double m_TimeOutDur = 0;
    double m_TimeRemained = 0;
    bool m_TimeOut = false;
    State m_State = State::STOP;
};
#endif
