#include "StageSelectScene.hpp"
#include "Util/Input.hpp"
#include "Sound.hpp"
#include "App.hpp"

StageSelectScene::StageSelectScene(App &app)
    : m_App(app) {
    m_BattleBtn = std::make_unique<GameButton>(
        RESOURCE_DIR "/buttons/StartButton.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/hover_purple.png",
             RESOURCE_DIR "/buttons/hover_yellow.png"}));
    m_BattleBtn->SetZIndex(5);
    m_BattleBtn->SetPosition(0.0f, -200.0f);
    m_BattleBtn->AddButtonEvent([this] {
        m_App.SwitchToBattleScene(StageFactory::CreateStage(Stages::LEVEL1));
    });
    m_Root.AddChild(m_BattleBtn);

    m_ReturnBtn = std::make_unique<GameButton>(
        RESOURCE_DIR "/buttons/button_back_ipad.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/button_back_yellow.png",
             RESOURCE_DIR "/buttons/button_back_purple.png"}));
    m_ReturnBtn->SetZIndex(5);
    m_ReturnBtn->SetPosition(
        float(app_w) / -2.0f + m_ReturnBtn->GetScaledSize().x / 2.0f + 60,
        float(app_h) / -2.0f + m_ReturnBtn->GetScaledSize().y / 2.0f);
    m_ReturnBtn->AddButtonEvent([this] {
        m_App.SwitchScene(App::SceneType::CAT_BASE);
        m_App.SwitchBGM(App::BGMType::CAT_BASE);
    });
    m_Root.AddChild(m_ReturnBtn);
}

void StageSelectScene::Update() {
    FrameScene::Update();
    m_Stages.Update();
    m_Stages.Draw();

    m_BattleBtn->Update();
    m_ReturnBtn->Update();

    m_Root.Update();
}
