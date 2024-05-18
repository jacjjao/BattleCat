#include "StageSelectScene.hpp"
#include "Util/Input.hpp"
#include "Sound.hpp"
#include "App.hpp"

StageSelectScene::StageSelectScene(App &app)
    : m_App(app) {
    auto background = std::make_shared<GameObjectEx>
        (std::make_unique<Util::Image>(RESOURCE_DIR"/img/img030_tw.png"),0);
    background->SetScale(1.2f,1.2f);
    m_Root.AddChild(background);
//-----------------------------------------------------------
    m_BattleBtn = std::make_unique<GameButton>(
        RESOURCE_DIR "/buttons/StartButton2.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/hover_purple2.png",
             RESOURCE_DIR "/buttons/hover_yellow2.png"}));
    m_BattleBtn->SetZIndex(5);
    m_BattleBtn->SetPosition(400.0f, -170.0f);
    m_BattleBtn->AddButtonEvent([this] {
        m_App.SwitchToBattleScene(StageFactory::CreateStage(static_cast<Stages>(m_Stages.GetCurStage())));
        m_App.SwitchBGM(App::BGMType::BATTLE);
    });
    m_Root.AddChild(m_BattleBtn);
//--------------------------------------------------------------------------------
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
        //m_App.SwitchBGM(App::BGMType::CAT_BASE);
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

void StageSelectScene::Enter() {
    m_Stages.Reset();
}
