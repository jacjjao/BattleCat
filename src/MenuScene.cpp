#include "pch.hpp"
#include "MenuScene.hpp"
#include "Util/Image.hpp"
#include "GameObjectEx.hpp"
#include "App.hpp"
#include "CatBase.hpp"

MenuScene::MenuScene(App &app) : m_App(app) {
    auto background = std::make_shared<GameObjectEx>();
    background->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/scene/Menu.png"));
    background->SetZIndex(0.0f);
    m_Root.AddChild(background);

    auto logo =  std::make_shared<GameObjectEx>();
    logo->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/scene/logo.png"));
    logo->SetZIndex(0.5f);
    logo->SetScale(0.6f, 0.6f);
    logo->SetPosition(0.0f, 200.0f);
    m_Root.AddChild(logo);

    m_StartButton = CreateGameYellowButton(RESOURCE_DIR"/buttons/YellowButton.png",
                                           {RESOURCE_DIR"/buttons/YellowButton_p.png",
                                            RESOURCE_DIR"/buttons/YellowButton_y.png"},
                                           RESOURCE_DIR"/sounds/click.mp3");
    m_StartButton->SetZIndex(0.6);
    m_StartButton->SetPosition(0.0f, -75.0f);
    m_StartButton->AddOnClickCallBack([this] { 
        m_App.SwitchScene(App::SceneType::CAT_BASE);
        m_App.SwitchBGM(App::BGMType::CAT_BASE);
    });

    m_Root.AddChild(m_StartButton);
}

void MenuScene::Update() {
    m_StartButton->Update();
    m_Root.Update();
}


