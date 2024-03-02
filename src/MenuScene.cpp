#include "pch.hpp"
#include "MenuScene.hpp"
#include "Util/Image.hpp"
#include "GameObjectEx.hpp"

MenuScene::MenuScene() {
    auto background = std::make_shared<GameObjectEx>();
    background->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/scene/Menu.png"));
    background->SetZIndex(0.0f);
    m_Root.AddChild(background);

    auto logo =  std::make_shared<GameObjectEx>();
    logo->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/scene/logo.png"));
    logo->SetZIndex(0.5f);
    logo->SetScale(0.85f, 0.85f);
    logo->SetPosition(0.0f, 150.0f);
    m_Root.AddChild(logo);

    m_StartButton =
        CreateGameYellowButton(RESOURCE_DIR "/scene/startButton.png",
                               {RESOURCE_DIR "/scene/hover_yellow.png",
                                RESOURCE_DIR "/scene/hover_purple.png"});
    m_StartButton->SetZIndex(0.6);
    m_StartButton->SetPosition(0.0f, -75.0f);
    m_StartButton->AddOnClickCallBack([] { 
        printf("Button clicked!\n");
    });

    m_Root.AddChild(m_StartButton);
}

void MenuScene::Update() {
    m_StartButton->Update();
    m_Root.Update();
}

