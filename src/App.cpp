#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

#include "MenuScene.hpp"
#include "CatBase.hpp"

void App::Start() {
    LOG_TRACE("Start");
    m_CurrentState = State::UPDATE;

    m_Scenes.emplace_back(static_cast<Scene*>(new MenuScene(*this)));
    m_Scenes.emplace_back(static_cast<Scene*>(new CatBaseScene(*this)));
    SwitchScene(SceneType::MENU);

    m_BGMs.push_back(std::make_unique<Util::BGM>(RESOURCE_DIR "/bgm/start.mp3"));
    m_BGMs.push_back(std::make_unique<Util::BGM>(RESOURCE_DIR "/bgm/base.mp3"));
    SwitchBGM(BGMType::MENU);
}

void App::Update() {
    
    //TODO: do your things here and delete this line <3
    m_CurScene->Update();
    /*
     * Do not touch the code below as they serve the purpose for
     * closing the window.
     */
    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) ||
        Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}

void App::SwitchScene(const SceneType type) {
    const auto index = static_cast<size_t>(type);
    if (index >= m_Scenes.size()) {
        throw std::invalid_argument("Invalid SceneType");
    }
    m_CurScene = m_Scenes[index].get();
}

void App::SwitchBGM(const BGMType type) {
    if (m_CurBGM) {
        m_CurBGM->Pause();
    }
    const auto index = static_cast<size_t>(type);
    if (index >= m_BGMs.size()) {
        throw std::invalid_argument("Invalid BGMType");
    }
    m_CurBGM = m_BGMs[index].get();
    m_CurBGM->Play();
}
