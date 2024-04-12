#include "UpgradeScene.hpp"
#include "App.hpp"

UpgradeScene::UpgradeScene(App &app) : m_App(app){
    auto back_button = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/button_back_ipad.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/button_back_yellow.png",
             RESOURCE_DIR "/buttons/button_back_purple.png"}));
    back_button->SetZIndex(3);
    back_button->SetPosition(float(app_w)/-2.0f + back_button->GetScaledSize().x/2.0f + 60,
                             float(app_h)/-2.0f + back_button->GetScaledSize().y/2.0f);
    back_button->AddButtonEvent([this] {
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
    m_catlist.reserve(MAXUNITS);
    for(int i=0;i<MAXUNITS;i++){
        auto &unit = m_catlist.emplace_back
                     (std::make_shared<UnitCard>(i,1.9f));
        m_Root.AddChild(unit);
    }
    //---------------------------------------------------------------------------------
    auto TransFormbtn = std::make_shared<GameButton>(RESOURCE_DIR"/buttons/transform.png");
    TransFormbtn->SetZIndex(2.2f);
    TransFormbtn->SetPosition(185.0f,-40.0f);
    TransFormbtn->AddButtonEvent([this]{
        LOG_DEBUG("Cat form transform in upgrade.");
    });
    m_Buttons.push_back(TransFormbtn);
    m_Root.AddChild(TransFormbtn);
    //---------------------------------------------------------------------------------
    SetBaseText(RESOURCE_DIR"/upgrade/basetext_upgrade.png");
    UpdateCatList();
};

void UpgradeScene::Update(){
    bool left = (Util::Input::IsKeyDown(Util::Keycode::LEFT) && m_currentunit > 0);
    bool right = (Util::Input::IsKeyDown(Util::Keycode::RIGHT) && m_currentunit < m_catlist.size()-1);
    m_currentunit += right - left;
    if(left || right){
        UpdateCatList();
    }
    //--------------------------------------------------------------------------------
    for (const auto &btn : m_Buttons) {
        btn->Update();
    }
    m_Root.Update();
};

void UpgradeScene::UpdateCatList(){
    for(int i=0;i < m_catlist.size();i++){
        auto &unit = m_catlist.at(i);
        if(i == m_currentunit){
            unit->SetPosition(0,112.0f);
            unit->SetScale(1.2f,1.2f);
            unit->SetVisible(true);
        }
        else if(std::abs(m_currentunit-i) <= 2){
            unit->SetPosition((i-m_currentunit)*285 + (i-m_currentunit>=0 ? 70:-70),60);
            unit->SetScale(0.8f,0.8f);
            unit->SetVisible(true);
        }
        else{
            unit->SetVisible(false);
        }
    }
}