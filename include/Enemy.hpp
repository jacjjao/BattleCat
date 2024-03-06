#pragma once

#include "EnemyAttr.hpp"
#include "EntityStats.hpp"
#include <functional>

enum class EnemyType : size_t {
    DOGE = 0
};
static_assert(std::is_same_v<std::underlying_type_t<EnemyType>, size_t>);

class Enemy {
public:
    Enemy(const EntityStats &stats, EnemyType type, float stats_modifier);

    void StartAttack(std::function<void(const Enemy&)> hit_callback);

    void GetHit(int damage);

private:
    EnemyAttr m_Attr;
    EntityState m_State = EntityState::WALK;
    EntityStats m_Stats;
};
