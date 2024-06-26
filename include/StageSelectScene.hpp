#ifndef STAGE_SELECT_SCENE_HPP
#define STAGE_SELECT_SCENE_HPP

#include "FrameScene.hpp"
#include "Util/Image.hpp"
#include "StageList.hpp"
#include "GameButton.hpp"

class StageSelectScene : public FrameScene
{
public:
    explicit StageSelectScene(App &app);

    void Update() override;

    void Enter() override;

private:
    App &m_App;

    StageList m_Stages;

    std::shared_ptr<GameButton> m_BattleBtn;
    std::shared_ptr<GameButton> m_ReturnBtn;
};

#endif // STAGE_SELECT_SCENE_HPP