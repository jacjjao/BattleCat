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
    static inline const unsigned int app_w = Core::Context::GetInstance()->GetWindowWidth();
    static inline const unsigned int app_h = Core::Context::GetInstance()->GetWindowHeight();
protected:

    enum SceneState{
        EXIT=0,
        UPDATE=1,
        ENTER=2
    };
    Util::Root m_Root;
    SceneState m_state = SceneState::EXIT;

    virtual void Enter(){};
    virtual void Exit(){};
};

#endif //BATTLECAT_SCENE_HPP
