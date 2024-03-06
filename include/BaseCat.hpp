#pragma once

#include "EnemyAttr.hpp"
#include "Entity.hpp"
#include <functional>

class BaseCat : public Entity {
public:
    virtual ~BaseCat() = default;

    virtual void StartAttack(std::function<void(const BaseCat&, EnemyAttr)>) = 0;

    virtual void GetHit(int damage, EnemyAttr attr) = 0;

    virtual float GetAtkModifier(EnemyAttr attr) const = 0;

protected:
    Entity m_Stat;
};
