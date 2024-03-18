#include "UpgradeScene.hpp"
#include "App.hpp"

UpgradeScene::UpgradeScene(App &app) : m_App(app){
    auto back_button = std::make_shared<GameButton>(
        RESOURCE_DIR "/buttons/button_back_ipad.png",
        std::initializer_list<std::string>(
            {RESOURCE_DIR "/buttons/button_back_yellow.png",
             RESOURCE_DIR "/buttons/button_back_purple.png"}));
    back_button->SetZIndex(3);
    back_button->SetPosition(float(app_w)/-2.0f + back_button->GetScaledSize().x/2.0f + 60,
                             float(app_h)/-2.0f + back_button->GetScaledSize().y/2.0f);
    back_button->AddButtonEvent([this] {
        m_App.SwitchScene(App::SceneType::CAT_BASE);
    });
    m_Buttons.push_back(back_button);
    m_Root.AddChild(back_button);

    Setframes(RESOURCE_DIR"/cat_base/basetext_upgrade.png");
};

void UpgradeScene::Update(){
    for (const auto &btn : m_Buttons) {
        btn->Update();
    }
    m_Root.Update();
};