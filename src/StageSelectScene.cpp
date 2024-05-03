#include "StageSelectScene.hpp"

StageSelectScene::StageSelectScene(App &app)
    : m_App(app) {}

void StageSelectScene::Update() {
    FrameScene::Update();
    m_Stages.Update();
    m_Stages.Draw();
}
