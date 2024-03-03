#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Scene.hpp"
#include "Util/BGM.hpp"
#include <optional>

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

    enum class BGMType : size_t {
        MENU = 0,
        CAT_BASE
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

    void SwitchScene(SceneType type);

    void SwitchBGM(std::optional<BGMType> type);

private:
    Util::BGM &GetBGM(BGMType type);

    State m_CurrentState = State::START;
    Scene* m_CurScene = nullptr;
    std::unique_ptr<Scene> m_MenuScene;
    std::unique_ptr<Scene> m_CatBaseScene;
    std::vector<std::unique_ptr<Util::BGM>> m_BGMs;
    std::optional<BGMType> m_BGMType = std::nullopt;
};

#endif
