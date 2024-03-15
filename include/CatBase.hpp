#pragma once

#include "Scene.hpp"
#include "Util/Root.hpp"
#include "GameButton.hpp"
#include <vector>

class CatBaseScene : public Scene {
public:
    CatBaseScene(App &app);

    void Update() override;

private:
    Util::Root m_Root;
    App &m_App;

    std::vector<std::shared_ptr<GameButton>> m_Buttons;
    std::shared_ptr<GameObjectEx> m_BaseCat;
};