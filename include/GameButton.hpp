#pragma once

#include "GameObjectEx.hpp"

class GameButton : public GameObjectEx{
public:
    void AddOnClickCallBack(const std::function<void()>& func);

    void Update();

private:
    bool IsClicked();

    std::vector<std::function<void()>> m_OnClickCallBacks;
};