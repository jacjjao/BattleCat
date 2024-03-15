#include "pch.hpp"
#include "MenuScene.hpp"
#include "Util/Image.hpp"
#include "GameObjectEx.hpp"
#include "App.hpp"
#include "CatBase.hpp"

#include "Util/Input.hpp"

MenuScene::MenuScene(App &app) : m_App(app) {
    auto background = std::make_shared<GameObjectEx>
        (std::make_unique<Util::Image>(RESOURCE_DIR"/scene/Menu.png"),0);
    m_Root.AddChild(background);

    auto blackbox = std::make_shared<GameObjectEx>
        (std::make_unique<Util::Image>(RESOURCE_DIR"/scene/Menu_blackbox.png"),0.1f);
    blackbox->SetPosition(0,blackbox->GetScaledSize().y/-1.8f);
    m_Root.AddChild(blackbox);

    auto logo =  std::make_shared<GameObjectEx>(std::make_unique<Util::Image>
        (RESOURCE_DIR"/scene/logo.png"),0.5f);
    logo->SetScale(0.7, 0.7f);
    logo->SetPosition(0.0f, 180.0f);
    m_Root.AddChild(logo);

    m_PlayButton = std::make_shared<GameButton>(RESOURCE_DIR"/buttons/PlayButton.png",
                                                 std::initializer_list<std::string>({RESOURCE_DIR"/buttons/hover_purple.png",
                                                  RESOURCE_DIR"/buttons/hover_yellow.png"}));
    m_PlayButton->SetZIndex(0.6f);
    m_PlayButton->SetPosition(0.0f, -75.0f);
    m_PlayButton->AddButtonEvent([this] {
        m_App.SwitchScene(App::SceneType::CAT_BASE);
        m_App.SwitchBGM(App::BGMType::CAT_BASE);
    });
    m_Root.AddChild(m_PlayButton);

    m_SettingsButton = std::make_shared<GameButton>(RESOURCE_DIR"/buttons/OptionButton.png",
                                                    std::initializer_list<std::string>({RESOURCE_DIR"/buttons/hover_purple.png",
                                                                                        RESOURCE_DIR"/buttons/hover_yellow.png"}));
    m_SettingsButton->SetZIndex(0.6f);
    m_SettingsButton->SetPosition(0.0f, -175.0f);
    //m_SettingsButton->SetText(RESOURCE_DIR"/scene/txt_option.png",0.8f);
    m_Root.AddChild(m_SettingsButton);

}

void MenuScene::Update() {
    m_PlayButton->Update();
    m_SettingsButton->Update();
    m_Root.Update();
}


