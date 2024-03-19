#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp" // IWYU pragma: export
#include "Scene.hpp"
#include "Util/BGM.hpp"

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    enum class SceneType : size_t { 
        MENU = 0,
        CAT_BASE,
        BATTLE_SCENE,
        EQUIP_SCENE,
        UPGRADE_SCENE
    };
    static_assert(std::is_same_v<std::underlying_type_t<SceneType>, size_t>); // Do not change the underlying type

    enum class BGMType : size_t {
        MENU = 0,
        CAT_BASE
    };
    static_assert(std::is_same_v<std::underlying_type_t<BGMType>, size_t>); // Do not change the underlying type

    State GetCurrentState() const { return m_CurrentState; }

    void Start();

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

    void SwitchScene(SceneType type);

    void SwitchBGM(BGMType type);

private:
    State m_CurrentState = State::START;

    std::vector<std::unique_ptr<Scene>> m_Scenes;
    Scene* m_CurScene = nullptr;

    std::vector<std::unique_ptr<Util::BGM>> m_BGMs;
    Util::BGM* m_CurBGM = nullptr;
};

#endif
