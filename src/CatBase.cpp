#include "pch.hpp"
#include "CatBase.hpp"
#include "Util/Image.hpp"
#include "GameObjectEx.hpp"
#include "Core/Context.hpp"
#include "App.hpp"
#include "GameButton.hpp"

CatBaseScene::CatBaseScene(App &app) 
    : m_App(app) {
    auto background = std::make_shared<GameObjectEx>();
    background->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR "/cat_base/background.png"));
    background->SetZIndex(0.0f);
    background->SetPosition(0.0f, 0.0f); // background�Ϥ��O�n��
    
    const auto bg_size = background->GetScaledSize();
    const auto app_w = Core::Context::GetInstance()->GetWindowWidth();
    const auto app_h = Core::Context::GetInstance()->GetWindowHeight();
    background->SetScale(float(app_w) / bg_size.x, float(app_h) / bg_size.y);

    m_Root.AddChild(background);

    
    m_BackBotton = CreateGameYellowButton(
        RESOURCE_DIR "/buttons/button_back_ipad.png",
        {RESOURCE_DIR "/buttons/button_back_yellow.png",
         RESOURCE_DIR "/buttons/button_back_purple.png"},
        RESOURCE_DIR"/sounds/click.mp3");
    m_BackBotton->SetZIndex(0.5f);
    m_BackBotton->SetPosition(-200.0f, -200.0f);
    m_BackBotton->AddOnClickCallBack([this] { 
        m_App.SwitchScene(App::SceneType::MENU);
    });

    m_Root.AddChild(m_BackBotton);
}

void CatBaseScene::Update() {
    m_BackBotton->Update();
    m_Root.Update();
}
