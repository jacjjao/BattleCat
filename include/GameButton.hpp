#pragma once

#include "GameObjectEx.hpp"
#include "AnimatedGameObject.hpp"
#include "Util/SFX.hpp"

class GameButton : public GameObjectEx{
public:
    //explicit GameButton() = default;
    explicit GameButton(const std::string &btn_path);

    explicit GameButton(const std::string &btn_path,std::initializer_list<std::string> border_paths);

    void SetClickSound(const std::function<void()>& click_sound);

    void AddButtonEvent(const std::function<void()>& func);

    void Update();

    void SetPosition(float x, float y);

    void SetScale(float scale);

    void SetZIndex(float index);

private:
    static inline std::unique_ptr<Util::SFX> s_ClickSound = nullptr;

    std::vector<std::function<void()>> m_ButtonEvents;

    std::shared_ptr<AnimatedGameObject> m_HoverBorder = nullptr;

    bool IsMouseHovering() const;
    void SetHoverBorder(std::shared_ptr<AnimatedGameObject> border);
};

/*std::shared_ptr<GameButton>
CreateGameYellowButton(const std::string &btn_path,
                       std::initializer_list<std::string> border_paths);*/
