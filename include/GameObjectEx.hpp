//
// Created by user on 2024/3/1.
//

#ifndef BATTLECAT_GAMEOBJECTEX_HPP
#define BATTLECAT_GAMEOBJECTEX_HPP

#include "Util/GameObject.hpp"

class GameObjectEx : public Util::GameObject{
public:
    GameObjectEx();
    void SetScale(float x, float y);
    void SetPosition(float x, float y);
};

#endif //BATTLECAT_GAMEOBJECTEX_HPP
