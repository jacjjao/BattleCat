//
// Created by user on 2024/3/19.
//

#ifndef BATTLECAT_UPGRADESCENE_HPP
#define BATTLECAT_UPGRADESCENE_HPP

#include "pch.hpp"
#include "FrameScene.hpp"
#include "GameButton.hpp"
#include "CatList.hpp"

class UpgradeScene : public FrameScene, private CatList {
public:
    UpgradeScene(App &app);
    void Update() override;

private:
    App &m_App;
    std::vector<std::shared_ptr<GameButton>> m_Buttons;

};
#endif // BATTLECAT_UPGRADESCENE_HPP
