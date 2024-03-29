#ifndef DEPLOY_BUTTON_HPP
#define DEPLOY_BUTTON_HPP

#include "GameButton.hpp"
#include "Timer.hpp"

class DeployButton : public GameButton {
public:
    using GameButton::GameButton;

    void Update(double dt) override;

    void SetCoolDownTime(double time);

private:
    Timer m_Timer;
};

#endif