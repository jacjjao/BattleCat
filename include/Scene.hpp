//
// Created by user on 2024/3/1.
//

#ifndef BATTLECAT_SCENE_HPP
#define BATTLECAT_SCENE_HPP


#include "Util/BGM.hpp"
#include "Core/Context.hpp"
#include "Util/Root.hpp"

class Scene{
public:
    virtual ~Scene() = default;
    virtual void Update() = 0;
    const unsigned int app_w = Core::Context::GetInstance()->GetWindowWidth();
    const unsigned int app_h = Core::Context::GetInstance()->GetWindowHeight();
protected:
    Util::Root m_Root;
};

#endif //BATTLECAT_SCENE_HPP
