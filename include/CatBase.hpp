#ifndef CATBASE_HPP
#define CATBASE_HPP

#pragma once

#include "FrameScene.hpp"
#include "Util/Root.hpp"
#include "GameButton.hpp"
#include <vector>

class CatBaseScene : public FrameScene {
public:
    CatBaseScene(App &app);
    virtual void Update() override;
private:
    App &m_App;
    std::vector<std::shared_ptr<GameButton>> m_Buttons;
    std::shared_ptr<GameObjectEx> m_BaseCat;
};
#endif //CATBASE_HPP
