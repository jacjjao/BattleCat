//
// Created by user on 2024/3/19.
//

#ifndef BATTLECAT_UPGRADESCENE_HPP
#define BATTLECAT_UPGRADESCENE_HPP

#include "pch.hpp"
#include "FrameScene.hpp"
#include "GameButton.hpp"
#include "CatList.hpp"

class UpgradeScene : public FrameScene{
public:
    UpgradeScene(App &app);

    void Update() override;

    void UpdateCatList();

private:
    App &m_App;
    std::vector<std::shared_ptr<GameButton>> m_Buttons;
    int m_currentunit;
    std::vector<std::shared_ptr<UnitCard>> m_catlist;
};
#endif // BATTLECAT_UPGRADESCENE_HPP
