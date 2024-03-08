#include "Entity.hpp"

void Entity::StartAttack() {
    m_State = EntityState::ON_ATTACK;
}

void Entity::SetStats(const EntityStats &stats) {
    m_Stats = stats;
}

void Entity::GetHit(int damage, std::optional<EnemyAttr> attr) {
    m_Stats.health -= damage;
}

int Entity::GetDamage(int damage, std::optional<EnemyAttr> attr) const {
    return damage;
}

std::optional<EnemyAttr> Entity::GetEnemyAttr() const {
    return m_Stats.attr;
}

HitBox Entity::GetHitBox() const {
    return {};
}
