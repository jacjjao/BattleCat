#pragma once

#include "GameObjectEx.hpp"
#include "AnimatedGameObject.hpp"
#include "Util/SFX.hpp"

class GameButton : public GameObjectEx{
public:
    //GameButton() = default;
    //GameButton(const std::string &btn_path,std::initializer_list<std::string> border_paths);

    void AddOnClickCallBack(const std::function<void()>& func);

    void Update();

    void SetHoverBorder(std::shared_ptr<AnimatedGameObject> border);

    void SetPosition(float x, float y);

    void SetZIndex(float index);

    void SetClickSound(const std::string &sound_path);

private:
    bool IsMouseHovering();

    std::vector<std::function<void()>> m_OnClickCallBacks;
    std::shared_ptr<AnimatedGameObject> m_HoverBorder;
    std::shared_ptr<GameObjectEx> m_button;
    std::shared_ptr<Util::SFX> m_sound;
};

std::shared_ptr<GameButton>
CreateGameYellowButton(const std::string &btn_path,
                       std::initializer_list<std::string> border_paths,
                       const std::string &sound_path);
