//
// Created by user on 2024/3/1.
//

#ifndef BATTLECAT_MENUSCENE_HPP
#define BATTLECAT_MENUSCENE_HPP

#include "Scene.hpp"
#include "Util/Root.hpp"
#include "GameButton.hpp"

class MenuScene : public Scene{
public:
    MenuScene();

    void Update() override;

private:
    Util::Root m_Root;
    std::shared_ptr<GameButton> m_StartButton;
};

#endif //BATTLECAT_MENUSCENE_HPP
