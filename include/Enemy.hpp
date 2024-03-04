#pragma once

#include "EnemyAttr.hpp"
#include "Entity.hpp"
#include <functional>

class Enemy : public Entity {
public:
    void StartAttack(std::function<void(const Enemy&)> hit_callback);

    virtual void GetHit(int damage) = 0;

protected:
    EnemyAttr m_Attr;
};
