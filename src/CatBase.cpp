#include "CatBase.hpp"
#include "App.hpp"
#include "GameButton.hpp"
#include "GameObjectEx.hpp"
#include "Util/Image.hpp"
#include "pch.hpp"

CatBaseScene::CatBaseScene(App &app) 
    : m_App(app) {
    auto door = std::make_shared<GameObjectEx>();
    door->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/cat_base/door.png"));
    //door->SetZIndex(0.0f);
    //door->SetPosition(0.0f, 0.0f);
    const auto bg_size = door->GetScaledSize();
    door->SetScale(float(app_w) / bg_size.x, float(app_h) / bg_size.y);
    m_Root.AddChild(door);
//-----------------------------------------------------------------------------
    auto back_button = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/button_back_ipad.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/button_back_yellow.png",
             RESOURCE_DIR "/buttons/button_back_purple.png"}));
    back_button->SetZIndex(3);
    back_button->SetPosition(float(app_w)/-2.0f + back_button->GetScaledSize().x/2.0f + 60,
                              float(app_h)/-2.0f + back_button->GetScaledSize().y/2.0f);
    back_button->AddButtonEvent([this] {
        m_App.SwitchScene(App::SceneType::MENU);
        m_App.SwitchBGM(App::BGMType::MENU);
    });
    m_Buttons.push_back(back_button);
    m_Root.AddChild(back_button);
//-----------------------------------------------------------------------------
    auto start_button = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/StartButton.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/hover_purple.png",
             RESOURCE_DIR "/buttons/hover_yellow.png"}));
    start_button->SetZIndex(3);
    start_button->SetPosition(-375.0f,150.0f);
    start_button->AddButtonEvent([this] {
        m_App.SwitchToBattleScene(StageFactory::CreateStage(Stages::LEVEL1));
    });
    m_Buttons.push_back(start_button);
    m_Root.AddChild(start_button);
//-----------------------------------------------------------------------------
    auto upgrade_button = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/UpgradeButton.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/hover_purple.png",
             RESOURCE_DIR "/buttons/hover_yellow.png"}));
    upgrade_button->SetZIndex(3);
    upgrade_button->SetPosition(-375.0f,60.0f);
    upgrade_button->AddButtonEvent([this]{
        m_App.SwitchScene(App::SceneType::UPGRADE_SCENE);
    });
    m_Buttons.push_back(upgrade_button);
    m_Root.AddChild(upgrade_button);
//----------------------------------------------------------------------
    auto equip_button = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/EquipButton.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/hover_purple.png",
             RESOURCE_DIR "/buttons/hover_yellow.png"}));
    equip_button->SetZIndex(3);
    equip_button->SetPosition(-375.0f,-30.0f);
    equip_button->AddButtonEvent([this]{
        m_App.SwitchScene(App::SceneType::EQUIP_SCENE);
    });
    m_Buttons.push_back(equip_button);
    m_Root.AddChild(equip_button);
//-----------------------------------------------------------------------------
    m_BaseCat = std::make_shared<GameObjectEx>(std::make_unique<Util::Image>(RESOURCE_DIR"/cat_base/Cat_OpenEyes.png"),0.08f);
    m_BaseCat->SetPosition(430,-210);
    m_BaseCat->SetScale(1.5,1.5);

    auto BaseCat_closed_eyes = std::make_shared<GameObjectEx>(std::make_unique<Util::Image>(RESOURCE_DIR"/cat_base/Cat_CloseEyes.png"),0.9f);
    BaseCat_closed_eyes->SetPosition(m_BaseCat->GetPosition());
    BaseCat_closed_eyes->SetScale(m_BaseCat->GetScale());
    m_BaseCat->AddChild(BaseCat_closed_eyes);
    m_Root.AddChild(m_BaseCat);
//----------------------------------------------------------------------------------
    auto DialogBox = std::make_shared<GameObjectEx>(std::make_unique<Util::Image>(RESOURCE_DIR"/cat_base/dialog_box.png"),2);
    DialogBox->SetPosition(305,80);
    DialogBox->SetScale(2,2);
    m_Root.AddChild(DialogBox);
//---------------------------------------------------------------------------------
    SetBaseText(RESOURCE_DIR"/cat_base/basetext_base.png");


/* !!! for debug, will remove !!! */
    /*
    auto stage_select_btn = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/StartButton.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/hover_purple.png",
             RESOURCE_DIR "/buttons/hover_yellow.png"}));
    stage_select_btn->SetZIndex(3.0f);
    stage_select_btn->AddButtonEvent(
        [this] { m_App.SwitchScene(App::SceneType::STAGE_SELECT_SCENE);
    });
    m_Buttons.push_back(stage_select_btn);
    m_Root.AddChild(stage_select_btn);
    */
}

void CatBaseScene::Update() {
    FrameScene::Update();
    //-----------------------------------------------------------
    static int basecatframe;
    basecatframe += 1;
    basecatframe = basecatframe%3000;
    switch (basecatframe%75) {
        case 10:case 13:case 16:
            m_BaseCat->MovePosition(0,-2);
            break;
        case 40:case 43:
            m_BaseCat->MovePosition(0,3);
            break;
    }
    m_BaseCat->GetChildren().at(0)->SetVisible(
    (basecatframe % 140 == 134 || basecatframe % 140 == 135 || basecatframe%140 == 129 || basecatframe%140 == 130));
    //----------------------------------------------------------------------
    for (const auto &btn : m_Buttons) {
        btn->Update();
    }
    m_Root.Update();
}
