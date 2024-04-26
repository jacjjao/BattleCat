#include "StageSelectScene.hpp"

StageSelectScene::StageSelectScene(App &app)
    : m_App(app),
      m_Background(RESOURCE_DIR "/img/img030_tw.png") {}

void StageSelectScene::Update() {
    FrameScene::Update();
    m_Background.Draw({}, 0.0f);
}
