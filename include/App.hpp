#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Scene.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    enum class SceneType {
        MENU,
        CAT_BASE
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

    void SwitchScene(SceneType type);

private:
    void ValidTask();

private:
    State m_CurrentState = State::START;
    Scene* m_CurScene = nullptr;
    std::unique_ptr<Scene> m_MenuScene;
    std::unique_ptr<Scene> m_CatBaseScene;
};

#endif
