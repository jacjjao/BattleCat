//
// Created by user on 2024/3/15.
//

#ifndef BATTLECAT_EQUIPSCENE_HPP
#define BATTLECAT_EQUIPSCENE_HPP

#include "Scene.hpp"
#include "GameButton.hpp"
#include "Util/Root.hpp"

class App;

class EquipScene : public Scene{
public:
    EquipScene(App &app);
    void Update() override;
private:
    Util::Root m_Root;
    App &m_App;

    std::vector<std::shared_ptr<GameButton>> m_Buttons;
};












#endif // BATTLECAT_EQUIPSCENE_HPP
