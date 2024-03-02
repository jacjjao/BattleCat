#pragma once

#include "Scene.hpp"
#include "Util/Root.hpp"
#include "GameButton.hpp"

class App;

class CatBaseScene : public Scene {
public:
    CatBaseScene(App &app);

    void Update() override;

private:
    Util::Root m_Root;
    App &m_App;

    std::shared_ptr<GameButton> m_BackBotton;
};