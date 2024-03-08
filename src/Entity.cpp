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

EntityState Entity::GetState() const {
    return m_State;
}

bool Entity::CanAttack(float pos) const {
    auto det_box_pos = m_Stats.det_box;
    det_box_pos.low += m_PosX;
    det_box_pos.high += m_PosX;
    return m_State == EntityState::WALK &&
           (det_box_pos.low <= pos && pos <= det_box_pos.high);
}

float Entity::GetPosX() const {
    return m_PosX;
}
