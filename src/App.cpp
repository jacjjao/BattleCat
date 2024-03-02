#include "App.hpp"

#include "Util/Image.hpp"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"

#include "MenuScene.hpp"
#include "CatBase.hpp"

void App::Start() {
    LOG_TRACE("Start");
    //std::make_shared<Util::BGM>(RESOURCE_DIR"/bgm/start.mp3")->Play();
    m_CurrentState = State::UPDATE;
    m_MenuScene = std::unique_ptr<Scene>(static_cast<Scene*>(new MenuScene(*this)));
    m_CatBaseScene = std::unique_ptr<Scene>(static_cast<Scene*>(new CatBaseScene(*this)));
    m_CurScene = m_MenuScene.get();

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

void App::SwitchScene(SceneType type) {
    switch (type) { 
    case SceneType::MENU:
        m_CurScene = m_MenuScene.get();
        break;

    case SceneType::CAT_BASE:
        m_CurScene = m_CatBaseScene.get();
        break;
    }
}
