#include "pch.hpp"
#include "EquipScene.hpp"
#include "Util/Image.hpp"
#include "GameObjectEx.hpp"
#include "App.hpp"
#include "Util/Input.hpp"
#include "AnimatedGameObject.hpp"
#include "Utility.hpp"
#include "EquipList.hpp"
#include "Sound.hpp"

EquipScene::EquipScene(App &app) : m_App(app){
    //Set cat list.
    m_catlist = CatList::GetCatList();
    for(auto &cat : m_catlist){
        m_Root.AddChild(cat);
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
//-----------------------------------------------------------------------
    //Set background.
    auto background = std::make_shared<GameObjectEx>
        (std::make_unique<Util::Image>(RESOURCE_DIR"/equip/background.png"),0);
    background->SetScale(1.01f*app_w/background->GetScaledSize().x,0.85f*app_h/background->GetScaledSize().y);
    m_Root.AddChild(background);
//----------------------------------------------------
    //Set the equip background image.
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
        m_state = SceneState::EXIT;
        m_App.SwitchScene(App::SceneType::CAT_BASE);
    });
    m_buttons.push_back(back_button);
    m_Root.AddChild(back_button);
//-----------------------------------------------------------------
    //Set cats form transform button.
    auto TransFormbtn = std::make_shared<GameButton>(RESOURCE_DIR"/buttons/transform.png");
    TransFormbtn->SetZIndex(2.2f);
    TransFormbtn->SetPosition(185.0f,-280.0f);
    TransFormbtn->SetClickSound([]{
       Sounds::ButtonClick->Play();
    });
    TransFormbtn->AddButtonEvent([this]{
        auto it = std::find_if(EquipList::m_equiplist.begin(),EquipList::m_equiplist.end(),
       [this](std::shared_ptr<EquipCard> &ec) {return ec->GetUnitNum() == m_catlist.at(m_currentunit)->GetUnitNum();});
        if (it != EquipList::m_equiplist.end()){
            (*it)->Transform();
        }
    });
    TransFormbtn->AddButtonEvent([this]{
        m_catlist.at(m_currentunit)->Transform();
    });
    m_buttons.push_back(TransFormbtn);
    m_Root.AddChild(TransFormbtn);
//-----------------------------------------------------------------
    //The others.
    SetBaseText(RESOURCE_DIR"/equip/basetext_equip.png");
    UpdateCatList();
}

void EquipScene::Update() {
    if(m_state == SceneState::EXIT){
        m_state = SceneState::UPDATE;
        m_currentunit = 0;
        UpdateCatList();
    }
//---------------------------------------------------------------------
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
    bool CanDrag = !std::any_of(EquipList::m_equiplist.begin(),EquipList::m_equiplist.end(),
                [this](std::shared_ptr<EquipCard> &ec){return ec->GetUnitNum() == m_catlist.at(m_currentunit)->GetUnitNum();});
    if(CanDrag){
        auto &CurrentUnit = m_catlist.at(m_currentunit);
        CurrentUnit->Drag();
        if(PosInRange(m_equip->GetTopLeftPos(),m_equip->GetBottomRightPos(),Util::Input::GetCursorPosition())){
            if(CurrentUnit->GetCurrentState() == Draggable::State::PUT_OFF){
                AddEquip(CurrentUnit->GetUnitNum(),CurrentUnit->Getform());
            }
            else{
                CurrentUnit->MinifyAnime();
            }
        }
        else {
            CurrentUnit->AmplifyAnime();
        }
    }
//--------------------------------------------------------
    for(short int i=0;i<EquipList::m_equiplist.size();i++) {
        auto &eq = EquipList::m_equiplist.at(i);
        eq->Drag();
        if(eq->GetCurrentState() == Draggable::State::PUT_OFF &&
            !PosInRange(m_equip->GetTopLeftPos(),m_equip->GetBottomRightPos(),Util::Input::GetCursorPosition())){
            RemoveEquip(i);
        }
    }
//----------------------------------------------------------
    m_Root.Update();
}

void EquipScene::AddEquip(const unsigned int unitnum,bool form) {
    if(EquipList::m_equiplist.size() >= MAXEQUIP){
        return;
    }
    EquipList::m_equiplist.emplace_back(std::make_unique<EquipCard>(unitnum,1.89f,form));
    UpdateEquip();
}

void EquipScene::UpdateEquip(){
    for(short int i=0;i<EquipList::m_equiplist.size();i++) {
        EquipList::m_equiplist.at(i)->SetPos(-241 + (i % 5) * 146, 202 - (i / 5) * 94);
    }
}

void EquipScene::RemoveEquip(int index) {
    EquipList::m_equiplist.erase( EquipList::m_equiplist.begin() + index);
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