//
// Created by user on 2024/3/15.
//

#ifndef BATTLECAT_EQUIPSCENE_HPP
#define BATTLECAT_EQUIPSCENE_HPP

#include "FrameScene.hpp"
#include "GameButton.hpp"
#include "CatList.hpp"

class App;

class EquipScene : public FrameScene, private CatList {
public:
    explicit EquipScene(App &app);
    void Update() override;
private:
    App &m_App;

    std::vector<std::shared_ptr<GameButton>> m_buttons;

    std::shared_ptr<GameObjectEx> m_equip;
    std::shared_ptr<AnimatedGameObject> m_border;

    void UpdateEquip();
    void AddEquip(unsigned int unitnum,bool form);
    void RemoveEquip(int index);

};


#endif // BATTLECAT_EQUIPSCENE_HPP
