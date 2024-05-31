#include "UpgradeScene.hpp"
#include "App.hpp"
#include "Sound.hpp"

UpgradeScene::UpgradeScene(App &app) : m_App(app){
    for(auto &cat : m_catlist){
        m_Root.AddChild(cat);
    }
    //---------------------------------------------------------------------------------
    auto back_button = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/button_back_ipad.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/button_back_yellow.png",
             RESOURCE_DIR "/buttons/button_back_purple.png"}));
    back_button->SetZIndex(3);
    back_button->SetPosition(float(app_w)/-2.0f + back_button->GetScaledSize().x/2.0f + 60,
                             float(app_h)/-2.0f + back_button->GetScaledSize().y/2.0f);
    back_button->AddButtonEvent([this] {
        m_state = SceneState::EXIT;
        m_App.SwitchScene(App::SceneType::CAT_BASE);
    });
    m_Buttons.push_back(back_button);
    m_Root.AddChild(back_button);
    //--------------------------------------------------------------------------------------
    auto background = std::make_shared<GameObjectEx>
        (std::make_unique<Util::Image>(RESOURCE_DIR"/upgrade/background.png"),0);
    background->SetScale(app_w/background->GetScaledSize().x,1.4f);
    background->SetPosition(0,180);
    m_Root.AddChild(background);
    //-------------------------------------------------------------------------------------
    auto textbox = std::make_shared<GameObjectEx>
        (std::make_unique<Util::Image>(RESOURCE_DIR"/upgrade/textbox.png"),0);
    textbox->SetPosition(10,-240);
    m_Root.AddChild(textbox);
    //---------------------------------------------------------------------------------
    //Set cats form transform button.
    m_TransFormbtn->SetPosition(185.0f,-40.0f);
    m_Buttons.push_back(m_TransFormbtn);
    m_Root.AddChild(m_TransFormbtn);
    //---------------------------------------------------------------------------------
    auto upgrade_btn = std::make_shared<GameButton> (
        RESOURCE_DIR "/buttons/UpgradeButton2.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/hover_purple2.png",
             RESOURCE_DIR "/buttons/hover_yellow2.png"}),false);
    upgrade_btn->SetZIndex(3.0f);
    upgrade_btn->SetPosition(-415.0f,-140.0f);
    upgrade_btn->AddButtonEvent([this]{
        auto &currentunit = m_catlist.at(m_currentunit);
        unsigned int requireXP = 1000*(currentunit->Getlvl() + 1);
        if(m_XP >= requireXP && currentunit->Addlvl()){
            Sounds::UsingItem->Play();
            m_XP -= requireXP;
        }
        else{
            Sounds::Blocked->Play();
        }
    });
    m_Buttons.push_back(upgrade_btn);
    m_Root.AddChild(upgrade_btn);
    //---------------------------------------------------------------------------------
    SetBaseText(RESOURCE_DIR"/upgrade/basetext_upgrade.png");
    UpdateCatList(112.0f);
};

void UpgradeScene::Update(){
    FrameScene::Update();
    if(m_state == SceneState::EXIT){
        m_state = SceneState::UPDATE;
        m_currentunit = 0;
        UpdateCatList(112.0f);
    }
    //--------------------------------------------------------------------
    bool left = (Util::Input::IsKeyDown(Util::Keycode::LEFT) && m_currentunit > 0);
    bool right = (Util::Input::IsKeyDown(Util::Keycode::RIGHT) && m_currentunit < m_catlist.size()-1);
    m_currentunit += right - left;
    if(left || right){
        Sounds::Scrolling->Play();
        UpdateCatList(112.0f);
    }
    //--------------------------------------------------------------------------------
    for (const auto &btn : m_Buttons) {
        btn->Update();
    }
    m_Root.Update();
    DrawNumber();
};