//
// Created by user on 2024/3/15.
//

#ifndef BATTLECAT_EQUIPSCENE_HPP
#define BATTLECAT_EQUIPSCENE_HPP

#include "FrameScene.hpp"
#include "GameButton.hpp"
#include "UnitCard.hpp"

class App;

class EquipScene : public FrameScene{
public:
    EquipScene(App &app);
    void Update() override;
private:
    App &m_App;

    std::vector<std::shared_ptr<GameButton>> m_buttons;
    std::vector<std::shared_ptr<GameObjectEx>> m_equiplist;
    std::vector<std::shared_ptr<UnitCard>> m_catlist;

    std::shared_ptr<GameObjectEx> m_equip;
    std::shared_ptr<AnimatedGameObject> m_border;
    int m_currentunit = 0;

};


#endif // BATTLECAT_EQUIPSCENE_HPP
