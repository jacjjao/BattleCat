#include "pch.hpp"
#include "MenuScene.hpp"
#include "Util/Image.hpp"
#include "GameObjectEx.hpp"

MenuScene::MenuScene() {
    auto background = std::make_shared<GameObjectEx>();
    background->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/scene/Menu.png"));
    background->SetZIndex(0.0f);
    m_Root.AddChild(background);

    auto logo =  std::make_shared<GameObjectEx>();
    logo->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/scene/logo.png"));
    logo->SetZIndex(0.5f);
    logo->SetScale(0.75, 0.75);
    logo->SetPosition(0.0f,200.0f);
    m_Root.AddChild(logo);

}

void MenuScene::Update() {

    m_Root.Update();

}

