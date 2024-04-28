#ifndef STAGE_SELECT_SCENE_HPP
#define STAGE_SELECT_SCENE_HPP

#include "FrameScene.hpp"
#include "Util/Image.hpp"

class StageSelectScene : public FrameScene
{
public:
    StageSelectScene(App &app);

    void Update() override;

private:
    Util::Image m_Background;
    App &m_App;
};

#endif // STAGE_SELECT_SCENE_HPP