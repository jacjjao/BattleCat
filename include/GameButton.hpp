#pragma once

#include "GameObjectEx.hpp"
#include "AnimatedGameObject.hpp"
#include "Util/SFX.hpp"

class GameButton : public GameObjectEx{
public:
    explicit GameButton();

    explicit GameButton(const std::function<void()>& click_sound);

    void AddOnClickCallBack(const std::function<void()>& func);

    void Update();

    void SetHoverBorder(std::shared_ptr<AnimatedGameObject> border);

    void SetPosition(float x, float y);

    void SetScale(float x, float y);

    void SetZIndex(float index);

private:
    static inline std::unique_ptr<Util::SFX> s_ClickSound = nullptr;

    bool IsMouseHovering() const;

    std::vector<std::function<void()>> m_OnClickCallBacks;
    std::shared_ptr<AnimatedGameObject> m_HoverBorder;
};

std::shared_ptr<GameButton>
CreateGameYellowButton(const std::string &btn_path,
                       std::initializer_list<std::string> border_paths);
