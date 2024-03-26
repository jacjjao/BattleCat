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
    //Set cat list.
    m_catlist.reserve(MAXUNITS);
    for(int i=0;i<MAXUNITS;i++){
        auto &unit = m_catlist.emplace_back
             (std::make_shared<UnitCard>(std::make_unique<Util::Image>(RESOURCE_DIR"/cats/unit.png"),1.9f));
        m_Root.AddChild(unit);
    }
//------------------------------------------------------------------------
    //Set border.
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
    //Set limit number of equip cap.
    m_equiplist.reserve(MAXEQUIP);
//-----------------------------------------------------------------------
    //Set background.
    auto background = std::make_shared<GameObjectEx>
        (std::make_unique<Util::Image>(RESOURCE_DIR"/equip/background.png"),0);
    background->SetScale(1.01f*app_w/background->GetScaledSize().x,0.85f*app_h/background->GetScaledSize().y);
    m_Root.AddChild(background);
//----------------------------------------------------
    //Set the equip image.
    m_equip = std::make_shared<GameObjectEx>
        (std::make_unique<Util::Image>(RESOURCE_DIR"/equip/equip.png"),1);
    m_equip->SetPosition(0,155);
    m_Root.AddChild(m_equip);
//-----------------------------------------------------------------
    //Set back button.
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
    //The others.
    SetBaseText(RESOURCE_DIR"/equip/basetext_equip.png");
    UpdateCatList();
}

void EquipScene::Update() {
    bool left = (Util::Input::IsKeyDown(Util::Keycode::LEFT) && m_currentunit > 0);
    bool right = (Util::Input::IsKeyDown(Util::Keycode::RIGHT) && m_currentunit < m_catlist.size()-1);
    m_currentunit += right - left;
    if(left || right){
        UpdateCatList();
    }
//--------------------------------------------------------------------
    for (const auto &btn : m_buttons) {
        btn->Update();
    }
//-------------------------------------------------------------------
    auto &CurrentUnit = m_catlist.at(m_currentunit);
    CurrentUnit->Drag();
    if(PosInRange(m_equip->GetTopLeftPos(),m_equip->GetBottomRightPos(),Util::Input::GetCursorPosition())){
        CurrentUnit->SetDragImgScale(0.5f,0.5f);
        if(CurrentUnit->GetCurrentState() == DragState::PUT_OFF){
            AddEquip();
        }
    }
    else{
        CurrentUnit->SetDragImgScale(1.4f,1.4f);
    }
//--------------------------------------------------------
    for(short int i=0;i<m_equiplist.size();i++) {
        auto &eq = m_equiplist.at(i);
        eq->Drag();
        if(eq->GetCurrentState() == DragState::PUT_OFF &&
            !PosInRange(m_equip->GetTopLeftPos(),m_equip->GetBottomRightPos(),Util::Input::GetCursorPosition())){
            RemoveEquip(i);
        }
    }
//----------------------------------------------------------
    m_Root.Update();
}

void EquipScene::AddEquip() {
    if(m_equiplist.size() >= MAXEQUIP){
        return;
    }
    auto &eq = m_equiplist.emplace_back(std::make_unique<UnitCard>
        (std::make_unique<Util::Image>(RESOURCE_DIR"/cats/000/uni000_f00.png"),1.1f, false));
    m_Root.AddChild(eq);
    UpdateEquip();
}

void EquipScene::UpdateEquip(){
    for(short int i=0;i<m_equiplist.size();i++) {
        m_equiplist.at(i)->SetPosition(-241 + (i % 5) * 146, 202 - (i / 5) * 94);
    }
}

void EquipScene::RemoveEquip(int index) {
    auto &erased = m_equiplist.at(index);
    m_Root.RemoveChild(erased);
    m_equiplist.erase(m_equiplist.begin() + index);
    UpdateEquip();
}

void EquipScene::UpdateCatList(){
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
}