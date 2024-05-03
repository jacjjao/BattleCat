#ifndef STAGE_SELECT_SCENE_HPP
#define STAGE_SELECT_SCENE_HPP

#include "FrameScene.hpp"
#include "Util/Image.hpp"
#include "GameButton.hpp"

class StageSelectScene : public FrameScene
{
public:
    explicit StageSelectScene(App &app);
    void UpdateStageList();

    void Update() override;

private:
    Util::Image m_Background;
    App &m_App;
    int m_currentstage = 0;
    std::vector<std::shared_ptr<GameObjectEx>> m_stagelist;
    std::vector<std::shared_ptr<GameButton>> m_Buttons;
};

#endif // STAGE_SELECT_SCENE_HPP