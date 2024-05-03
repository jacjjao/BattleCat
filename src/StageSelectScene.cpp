#include "StageSelectScene.hpp"
#include "Util/Input.hpp"
#include "Sound.hpp"
#include "App.hpp"

StageSelectScene::StageSelectScene(App &app)
    : m_Background(RESOURCE_DIR "/img/img030_tw.png"),m_App(app) {
    {
        const int stagecount = 10;
        m_stagelist.reserve(10);
        for(unsigned int i=0;i<stagecount;i++){
            auto &stage =
                m_stagelist.emplace_back(std::make_shared<GameObjectEx>(std::make_unique<Util::Image>(RESOURCE_DIR"/stages/stagecard.png"),1.0f));
            m_Root.AddChild(stage);
        }
    }

    {
        auto back_button = std::make_shared<GameButton>(
            RESOURCE_DIR "/buttons/button_back_ipad.png",
            std::initializer_list<std::string>(
                {RESOURCE_DIR "/buttons/button_back_yellow.png",
                 RESOURCE_DIR "/buttons/button_back_purple.png"}));
        back_button->SetZIndex(3);
        back_button->SetPosition(float(app_w)/-2.0f + back_button->GetScaledSize().x/2.0f + 60,
                                 float(app_h)/-2.0f + back_button->GetScaledSize().y/2.0f);
        back_button->AddButtonEvent([this] {
            m_state = SceneState::EXIT;
            m_App.SwitchScene(App::SceneType::CAT_BASE);
        });
        m_Buttons.push_back(back_button);
        m_Root.AddChild(back_button);
    }

    UpdateStageList();
}

void StageSelectScene::Update() {
    FrameScene::Update();
    {
        Util::Transform t;
        t.scale = glm::vec2(1.2f,1.2f);
        m_Background.Draw(t, 0.0f);
    }

    {
        bool left = (Util::Input::IsKeyDown(Util::Keycode::LEFT) && m_currentstage > 0);
        bool right = (Util::Input::IsKeyDown(Util::Keycode::RIGHT) && m_currentstage < m_stagelist.size()-1);
        m_currentstage += right - left;
        if(left || right){
            Sounds::Scrolling->Play();
            UpdateStageList();
        }
    }

    {//Update objects.
        for (const auto &btn : m_Buttons) {
            btn->Update();
        }
        m_Root.Update();
    }

}

void StageSelectScene::UpdateStageList(){
    const float y = 250.0f;
    for(unsigned short i=0;i < short(m_stagelist.size());i++){
        auto &unit = m_stagelist.at(i);
        if(i == m_currentstage){
            unit->SetPosition(0,y);
            unit->SetScale(0.9f,0.9f);
            unit->SetVisible(true);
        }
        else if(std::abs(m_currentstage-i) <= 2){
            unit->SetPosition(float(i-m_currentstage)*200.0f + (i-m_currentstage>=0 ? 50.0f:-50.0f),y + 7);
            unit->SetScale(0.7f,0.7f);
            unit->SetVisible(true);
        }
        else{
            unit->SetVisible(false);
        }
    }
}
