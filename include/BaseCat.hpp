#pragma once

#include "EnemyAttr.hpp"
#include "Entity.hpp"

class BaseCat : public Entity {
public:
    virtual ~BaseCat() = default;

    virtual void Attack(EnemyAttr attr) = 0;

    virtual void GetHit(int damage, EnemyAttr attr) = 0;

    virtual float GetAtkModifier() const = 0;
};
