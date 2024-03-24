#include "pch.hpp"
#include "EquipScene.hpp"
#include "Util/Image.hpp"
#include "GameObjectEx.hpp"
#include "App.hpp"
#include "Util/Input.hpp"
#include "AnimatedGameObject.hpp"
#include "Utility.hpp"
#define MAXUNITS 25
#define MAXEQUIP 10

EquipScene::EquipScene(App &app) : m_App(app){
    m_catlist.reserve(MAXUNITS);
    for(int i=0;i<MAXUNITS;i++){
        auto unit = std::make_shared<UnitCard>
            (std::make_unique<Util::Image>(RESOURCE_DIR"/cats/unit.png"),1.9f);
        //unit->SetPosition(0,-180);

        m_catlist.push_back(unit);
        m_Root.AddChild(unit);
    }
//------------------------------------------------------------------------
    m_border = std::make_shared<AnimatedGameObject>
        (std::initializer_list<std::string>{RESOURCE_DIR"/cats/unit_purple.png",
                                            RESOURCE_DIR"/cats/unit_yellow.png"});
    m_border->SetZIndex(2.1f);
    m_border->SetPosition(0,-128);
    m_border->SetScale(1.2f,1.2f);
    m_border->SetInterval(67);
    m_border->SetLooping(true);
    m_border->SetVisible(true);
    m_border->Play();
    m_Root.AddChild(m_border);
//------------------------------------------------------------------------
    m_equiplist.reserve(MAXEQUIP);
//-----------------------------------------------------------------------
    auto background = std::make_shared<GameObjectEx>
        (std::make_unique<Util::Image>(RESOURCE_DIR"/equip/background.png"),0);
    background->SetScale(1.01f*app_w/background->GetScaledSize().x,0.85f*app_h/background->GetScaledSize().y);
    m_Root.AddChild(background);
//----------------------------------------------------
    m_equip = std::make_shared<GameObjectEx>
        (std::make_unique<Util::Image>(RESOURCE_DIR"/equip/equip.png"),1);
    m_equip->SetPosition(0,155);
    m_Root.AddChild(m_equip);
//-----------------------------------------------------------------
    auto back_button = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/button_back_ipad.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/button_back_yellow.png",
             RESOURCE_DIR "/buttons/button_back_purple.png"}));
    back_button->SetZIndex(3);
    back_button->SetPosition(app_w/-2.0f + back_button->GetScaledSize().x/2.0f + 60,
                             app_h/-2.0f + back_button->GetScaledSize().y/2.0f);
    back_button->AddButtonEvent([this] {
        m_App.SwitchScene(App::SceneType::CAT_BASE);
    });
    m_buttons.push_back(back_button);
    m_Root.AddChild(back_button);
//-----------------------------------------------------------------
    SetBaseText(RESOURCE_DIR"/equip/basetext_equip.png");
}

void EquipScene::Update() {
    if(Util::Input::IsKeyDown(Util::Keycode::LEFT) && m_currentunit > 0){
        m_currentunit -=1;
    }
    else if(Util::Input::IsKeyDown(Util::Keycode::RIGHT) && m_currentunit < m_catlist.size()-1){
        m_currentunit +=1;
    }
    for(int i=0;i < m_catlist.size();i++){
        auto &unit = m_catlist.at(i);
        if(i == m_currentunit){
            unit->SetPosition(m_border->GetPosition());
            unit->SetScale(1.2f,1.2f);
            unit->SetVisible(true);
        }
        else if(std::abs(m_currentunit-i) <= 2){
            unit->SetPosition((i-m_currentunit)*285 + (i-m_currentunit>=0 ? 70:-70),-180);
            unit->SetScale(0.8f,0.8f);
            unit->SetVisible(true);
        }
        else{
            unit->SetVisible(false);
        }
    }

    for (const auto &btn : m_buttons) {
        btn->Update();
    }
//-------------------------------------------------------------------
    auto CurrentUnit = m_catlist.at(m_currentunit);
    CurrentUnit->Dragging();
    if(CurrentUnit->GetState() == UnitCardState::PUT_OFF){
        if(m_equiplist.size() < MAXEQUIP &&
            PosInRange(m_equip->GetTopLeftPos(),m_equip->GetBottomRightPos(),Util::Input::GetCursorPosition())){
            auto &eq = m_equiplist.emplace_back(std::make_shared<GameObjectEx>(
                std::make_unique<Util::Image>(RESOURCE_DIR "/cats/000/uni000_f00.png"),1.1f));
            m_Root.AddChild(eq);
        }
        for(int i=0;i<m_equiplist.size();i++) {
            m_equiplist.at(i)->SetPosition(-241 + (i % 5) * 146, 202 - (i / 5) * 94);
        }
    }
//--------------------------------------------------------
    m_Root.Update();
}