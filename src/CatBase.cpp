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

    m_BackButton = CreateGameYellowButton(
        RESOURCE_DIR "/buttons/button_back_ipad.png",
        {RESOURCE_DIR "/buttons/button_back_yellow.png",
         RESOURCE_DIR "/buttons/button_back_purple.png"},
        RESOURCE_DIR"/sounds/click.mp3");
    m_BackButton->SetZIndex(0.5f);
    m_BackButton->SetPosition(float(app_w)/-2.0f + m_BackButton->GetScaledSize().x/2.0f + 74,
                              float(app_h)/-2.0f + m_BackButton->GetScaledSize().y/2.0f);
    m_BackButton->AddOnClickCallBack([this] {
        m_App.SwitchScene(App::SceneType::MENU);
        m_App.SwitchBGM(App::BGMType::MENU);
    });

    m_Root.AddChild(m_BackButton);
}

void CatBaseScene::Update() {
    m_BackButton->Update();
    m_Root.Update();
}
