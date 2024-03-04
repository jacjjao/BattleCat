//
// Created by user on 2024/3/1.
//

#ifndef BATTLECAT_GAMEOBJECTEX_HPP
#define BATTLECAT_GAMEOBJECTEX_HPP

#include "Util/GameObject.hpp"

class GameObjectEx : public Util::GameObject{
public:
    GameObjectEx();
    virtual void SetScale(float x, float y);
    virtual void SetPosition(float x, float y);
    virtual void SetPosition(glm::vec2 pos);
    glm::vec2 GetScale();
    glm::vec2 GetPosition();

};

#endif //BATTLECAT_GAMEOBJECTEX_HPP
