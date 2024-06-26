#ifndef DEPLOY_BUTTON_HPP
#define DEPLOY_BUTTON_HPP

#include "GameButton.hpp"
#include "Timer.hpp"
#include "Util/Text.hpp"
#include "Util/Root.hpp"
#include "LoadingRectangle.hpp"
#include <optional>
#include "NumberSystem.hpp"

class DeployButton : public GameButton {
public:
    using GameButton::GameButton;

    void Update(double dt) override;

    void SetCoolDownTime(double time);

    void SetCost(int cost);

    void Draw();

    void DrawCost();

    void StartCoolDown();

    static void Init();

    static void DrawStates();

private:
    static inline std::optional<LoadingRectangles> s_Renderer;

    Timer m_Timer;
    unsigned short int n_Cost{};
};

#endif