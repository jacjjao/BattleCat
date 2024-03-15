#include "pch.hpp"
#include "EquipScene.hpp"
#include "Util/Image.hpp"
#include "GameObjectEx.hpp"
#include "Core/Context.hpp"
#include "App.hpp"
#include "GameButton.hpp"

EquipScene::EquipScene(App &app): m_App(app) {
    const auto app_w = Core::Context::GetInstance()->GetWindowWidth();
    const auto app_h = Core::Context::GetInstance()->GetWindowHeight();
//-----------------------------------------------------------------------
    auto background = std::make_shared<GameObjectEx>
        (std::make_unique<Util::Image>(RESOURCE_DIR"/equip/background.png"),0);
    m_Root.AddChild(background);
//----------------------------------------------------
    auto equip = std::make_shared<GameObjectEx>
        (std::make_unique<Util::Image>(RESOURCE_DIR"/equip/equip.png"),1);
    equip->SetPosition(0,180);
    m_Root.AddChild(equip);
//-----------------------------------------------------------------
    auto back_button = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/button_back_ipad.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/button_back_yellow.png",
             RESOURCE_DIR "/buttons/button_back_purple.png"}));
    back_button->SetZIndex(3);
    back_button->SetPosition(float(app_w)/-2.0f + back_button->GetScaledSize().x/2.0f + 60,
                             float(app_h)/-2.0f + back_button->GetScaledSize().y/2.0f);
    back_button->AddButtonEvent([this] {
        m_App.SwitchScene(App::SceneType::BATTLE_SCENE);
    });
    m_Buttons.push_back(back_button);
    m_Root.AddChild(back_button);
}

void EquipScene::Update() {
    for (const auto &btn : m_Buttons) {
        btn->Update();
    }
    m_Root.Update();
}