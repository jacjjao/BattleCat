//
// Created by user on 2024/3/15.
//

#ifndef BATTLECAT_EQUIPSCENE_HPP
#define BATTLECAT_EQUIPSCENE_HPP

#include "FrameScene.hpp"
#include "GameButton.hpp"

class App;

class EquipScene : public FrameScene{
public:
    EquipScene(App &app);
    void Update() override;
private:
    App &m_App;

    std::vector<std::shared_ptr<GameButton>> m_Buttons;
    std::vector<std::shared_ptr<GameObjectEx>> m_equip;

};


#endif // BATTLECAT_EQUIPSCENE_HPP
