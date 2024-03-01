//
// Created by user on 2024/3/1.
//

#ifndef BATTLECAT_MENUSCENE_HPP
#define BATTLECAT_MENUSCENE_HPP

#include "Scene.hpp"
#include "Util/Root.hpp"

class MenuScene : public Scene{
public:
    MenuScene();

    void Update() override;

private:
    Util::Root m_Root;
};

#endif //BATTLECAT_MENUSCENE_HPP
