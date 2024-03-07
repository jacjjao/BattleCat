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

    auto logo =  std::make_shared<GameObjectEx>(std::make_unique<Util::Image>
        (RESOURCE_DIR"/scene/logo.png"),0.5f);
    logo->SetScale(0.6f, 0.6f);
    logo->SetPosition(0.0f, 200.0f);
    m_Root.AddChild(logo);

    m_PlayButton = std::make_shared<GameButton>(RESOURCE_DIR"/buttons/YellowButton.png",
                                                 std::initializer_list<std::string>({RESOURCE_DIR"/buttons/hover_purple.png",
                                                  RESOURCE_DIR"/buttons/hover_yellow.png"}));
    m_PlayButton->SetZIndex(0.6f);
    m_PlayButton->SetPosition(0.0f, -75.0f);
    //m_PlayButton->SetText(RESOURCE_DIR"/scene/txt_option.png",0.8f);
    m_PlayButton->AddButtonEvent([this] {
        m_App.SwitchScene(App::SceneType::CAT_BASE);
        m_App.SwitchBGM(App::BGMType::CAT_BASE);
    });
    m_Root.AddChild(m_PlayButton);

    m_SettingsButton = std::make_shared<GameButton>(RESOURCE_DIR"/buttons/YellowButton.png",
                                                    std::initializer_list<std::string>({RESOURCE_DIR"/buttons/hover_purple.png",
                                                                                        RESOURCE_DIR"/buttons/hover_yellow.png"}));
    m_SettingsButton->SetZIndex(0.6f);
    m_SettingsButton->SetPosition(0.0f, -175.0f);
    m_SettingsButton->SetText(RESOURCE_DIR"/scene/txt_option.png",0.8f);
    m_Root.AddChild(m_SettingsButton);

}

void MenuScene::Update() {
    //TODO: This just test,you can delete it.
    /**
    if (Util::Input::IsKeyDown(Util::Keycode::UP)){
        m_SettingsButton->MovePosition(0,10);
    }
    if (Util::Input::IsKeyDown(Util::Keycode::DOWN)){
        m_SettingsButton->MovePosition(0,-10);
    }
    if (Util::Input::IsKeyDown(Util::Keycode::LEFT)){
        m_SettingsButton->MovePosition(-10,0);
    }
    if (Util::Input::IsKeyDown(Util::Keycode::RIGHT)){
        m_SettingsButton->MovePosition(10,0);
    }
    if (Util::Input::IsKeyDown(Util::Keycode::BACKSPACE)){
        m_SettingsButton->AddRotation(1);
    }**/
    m_PlayButton->Update();
    m_SettingsButton->Update();
    m_Root.Update();
}


