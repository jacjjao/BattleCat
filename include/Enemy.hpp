#pragma once

#include "EnemyAttr.hpp"
#include "Entity.hpp"
#include <functional>

class Enemy {
public:
    virtual void StartAttack(std::function<void(const Enemy&)> hit_callback) = 0;

    virtual void GetHit(int damage) = 0;

protected:
    EnemyAttr m_Attr;
    Entity m_Stat;
};
