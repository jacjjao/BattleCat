#include "pch.hpp"
#include "CatBase.hpp"
#include "Util/Image.hpp"
#include "GameObjectEx.hpp"
#include "Core/Context.hpp"
#include "App.hpp"
#include "GameButton.hpp"

CatBaseScene::CatBaseScene(App &app) 
    : m_App(app) {
    const auto app_w = Core::Context::GetInstance()->GetWindowWidth();
    const auto app_h = Core::Context::GetInstance()->GetWindowHeight();

    auto door = std::make_shared<GameObjectEx>();
    door->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/cat_base/door.png"));
    //door->SetZIndex(0.0f);
    //door->SetPosition(0.0f, 0.0f);

    auto frame_L = std::make_shared<GameObjectEx>();
    auto frame_R = std::make_shared<GameObjectEx>();
    auto frame_UP = std::make_shared<GameObjectEx>();
    auto frame_DOWN = std::make_shared<GameObjectEx>();
    auto frame_base = std::make_shared<GameObjectEx>();

    frame_L->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/cat_base/frame_left.png"));
    frame_R->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/cat_base/frame_right.png"));
    frame_UP->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/cat_base/frame_up.png"));
    frame_DOWN->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/cat_base/frame_down.png"));
    frame_base->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/cat_base/frame_up_base.png"));

    frame_L->SetZIndex(0.2f);
    frame_R->SetZIndex(0.2f);
    frame_UP->SetZIndex(0.1f);
    frame_DOWN->SetZIndex(0.1f);
    frame_base->SetZIndex(0.15f);

    frame_L->SetScale(1,2);
    frame_R->SetScale(1,2);
    frame_UP->SetScale(2,1);
    frame_DOWN->SetScale(2,1);

    frame_L->SetPosition(float(app_w)/-2.0f + 37,0);
    frame_R->SetPosition(float(app_w)/2.0f - 37,0);
    frame_UP->SetPosition(0,float(app_h)/2.0f - frame_UP->GetScaledSize().y/2.0f);
    frame_DOWN->SetPosition(0,float(app_h)/-2.0f + frame_DOWN->GetScaledSize().y/2.0f);
    frame_base->SetPosition(float(app_w)/-2.0f + frame_base->GetScaledSize().x/2.0f + 74
                            ,float(app_h)/2.0f - frame_base->GetScaledSize().y/2.0f);

    m_Root.AddChild(frame_L);
    m_Root.AddChild(frame_R);
    m_Root.AddChild(frame_UP);
    m_Root.AddChild(frame_DOWN);
    m_Root.AddChild(frame_base);

    const auto bg_size = door->GetScaledSize();
    door->SetScale(float(app_w) / bg_size.x, float(app_h) / bg_size.y);

    m_Root.AddChild(door);

    auto back_button = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/button_back_ipad.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/button_back_yellow.png",
             RESOURCE_DIR "/buttons/button_back_purple.png"}));
    back_button->SetZIndex(0.5f);
    back_button->SetPosition(float(app_w)/-2.0f + back_button->GetScaledSize().x/2.0f + 74,
                              float(app_h)/-2.0f + back_button->GetScaledSize().y/2.0f);
    back_button->AddButtonEvent([this] {
        m_App.SwitchScene(App::SceneType::MENU);
        m_App.SwitchBGM(App::BGMType::MENU);
    });
    m_Buttons.push_back(back_button);
    m_Root.AddChild(back_button);

    auto start_button = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/YellowButton.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/hover_purple.png",
             RESOURCE_DIR "/buttons/hover_yellow.png"}));
    start_button->SetZIndex(0.5f);
    start_button->SetPosition(-400.0f,150.0f);
    m_Buttons.push_back(start_button);
    m_Root.AddChild(start_button);

    auto upgrade_button = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/YellowButton.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/hover_purple.png",
             RESOURCE_DIR "/buttons/hover_yellow.png"}));
    upgrade_button->SetZIndex(0.5f);
    upgrade_button->SetPosition(-400.0f,50.0f);
    m_Buttons.push_back(upgrade_button);
    m_Root.AddChild(upgrade_button);

    auto equip_button = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/YellowButton.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/hover_purple.png",
             RESOURCE_DIR "/buttons/hover_yellow.png"}));
    equip_button->SetZIndex(0.5f);
    equip_button->SetPosition(-400.0f,-50.0f);
    m_Buttons.push_back(equip_button);
    m_Root.AddChild(equip_button);
}

void CatBaseScene::Update() {
    for (const auto &btn : m_Buttons) {
        btn->Update();
    }
    m_Root.Update();
}
