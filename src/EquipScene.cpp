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
    //m_catlist = CatList::GetCatList();
    for(auto &cat : m_catlist){
        m_Root.AddChild(cat);
    }
//-------------------------------------------------
    //Equip must non-empty,init one cat.
    EquipScene::AddEquip(m_catlist.at(0));
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
    background->SetScale(1.01f*float(app_w)/background->GetScaledSize().x,0.85f*float(app_h)/background->GetScaledSize().y);
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
    back_button->SetPosition(float(app_w)/-2.0f + back_button->GetScaledSize().x/2.0f + 60,
                             float(app_h)/-2.0f + back_button->GetScaledSize().y/2.0f);
    back_button->AddButtonEvent([this] {
        if(EquipList::Size < 1){
            return;
        }
        m_state = SceneState::EXIT;
        m_App.SwitchScene(App::SceneType::CAT_BASE);
    });
    m_buttons.push_back(back_button);
    m_Root.AddChild(back_button);
//-----------------------------------------------------------------
    //Set cats form transform button.
    m_TransFormbtn->SetPosition(185.0f,-280.0f);
    m_buttons.push_back(m_TransFormbtn);
    m_Root.AddChild(m_TransFormbtn);
//-----------------------------------------------------------------
    //Misc.
    SetBaseText(RESOURCE_DIR"/equip/basetext_equip.png");
    UpdateCatList(-128.0f);
}

void EquipScene::Update() {
    FrameScene::Update();
    if(m_state == SceneState::EXIT){
        m_state = SceneState::UPDATE;
        m_currentunit = 0;
        UpdateCatList(-128.0f);
    }
//---------------------------------------------------------------------
    bool left = (Util::Input::IsKeyDown(Util::Keycode::LEFT) && m_currentunit > 0);
    bool right = (Util::Input::IsKeyDown(Util::Keycode::RIGHT) && m_currentunit < m_catlist.size()-1);
    m_currentunit += right - left;
    if(left || right){
        Sounds::Scrolling->Play();
        UpdateCatList(-128.0f);
    }
//--------------------------------------------------------------------
    for (const auto &btn : m_buttons) {
        btn->Update();
    }
//-------------------------------------------------------------------
    auto &CurrentUnit = m_catlist.at(m_currentunit);
    if(!CurrentUnit->Inuse()){
        CurrentUnit->Drag();
        if(PosInRange(m_equip->GetTopLeftPos(),m_equip->GetBottomRightPos(),Util::Input::GetCursorPosition())){
            if(CurrentUnit->GetCurrentState() == Draggable::DragState::PUT_OFF){
                AddEquip(CurrentUnit);
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
    for(unsigned short int i=0 ; i < EquipList::Size ; i++) {
        auto &eq = EquipList::m_equiplist.at(i);
        eq->Drag();
        if(eq->GetCurrentState() == Draggable::DragState::PUT_OFF &&
            !PosInRange(m_equip->GetTopLeftPos(),m_equip->GetBottomRightPos(),Util::Input::GetCursorPosition())){
            RemoveEquip(i);
        }
    }
//----------------------------------------------------------
    m_Root.Update();

    //test
    /*{
        bool k1 = Util::Input::IsKeyDown(Util::Keycode::KP_1);
        auto equipget = EquipList::GetEquipList();
        if(k1){
            printf("Unit: %d\n",static_cast<int>(equipget.at(0)->GetCatType()));
        }

    }*/
}

void EquipScene::AddEquip(std::shared_ptr<UnitCard>& unit) {
    if(EquipList::Size >= MAXEQUIP){
        return;
    }
    auto &eq = EquipList::m_equiplist.emplace_back(std::make_unique<EquipCard>(unit->GetUnitNum(),1.89f,unit->Getform()));
    unit->m_EquipCard = eq;
    eq->m_UnitCard = unit;
    EquipList::Size++;
    UpdateEquip();
}

void EquipScene::UpdateEquip(){
    for(unsigned short i = 0; i < EquipList::Size;i++) {
        EquipList::m_equiplist.at(i)->SetPos(-241.0f + float(i % 5) * 146.0f, 202.0f - (i / 5) * 94.0f);
    }
}

void EquipScene::RemoveEquip(int index) {
    EquipList::m_equiplist.erase( EquipList::m_equiplist.begin() + index);
    EquipList::Size--;
    UpdateEquip();
}
