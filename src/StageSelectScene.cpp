#include "StageSelectScene.hpp"

StageSelectScene::StageSelectScene(App &app)
    : m_Background(RESOURCE_DIR "/img/img030_tw.png"),
      m_App(app) {}

void StageSelectScene::Update() {
    FrameScene::Update();
    m_Background.Draw({}, 0.0f);
}
