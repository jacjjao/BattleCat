#include "StageSelectScene.hpp"
#include "Util/Input.hpp"
#include "Sound.hpp"
#include "App.hpp"

StageSelectScene::StageSelectScene(App &app)
    : m_App(app) {}

void StageSelectScene::Update() {
    FrameScene::Update();
    m_Stages.Update();
    m_Stages.Draw();
}
