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

    m_NextSceneBGM = std::make_shared<Util::BGM>(RESOURCE_DIR"/bgm/start.mp3");

    m_BackButton = CreateGameYellowButton(
        RESOURCE_DIR "/buttons/button_back_ipad.png",
        {RESOURCE_DIR "/buttons/button_back_yellow.png",
         RESOURCE_DIR "/buttons/button_back_purple.png"},
        RESOURCE_DIR"/sounds/click.mp3");
    m_BackButton->SetZIndex(0.5f);
    m_BackButton->SetPosition(-200.0f, -200.0f);
    m_BackButton->AddOnClickCallBack([this] {
        m_App.SwitchScene(App::SceneType::MENU);
        m_NextSceneBGM->Play();
    });

    m_Root.AddChild(m_BackButton);
}

void CatBaseScene::Update() {
    m_BackButton->Update();
    m_Root.Update();
}
