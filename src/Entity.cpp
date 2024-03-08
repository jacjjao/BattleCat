#include "Entity.hpp"

void Entity::StartAttack() {
    m_State = EntityState::ON_ATTACK;
#ifdef ENABLE_BATTLE_LOG
    printf("%s StarAttack at position: %.2f\n", m_Stats.name.c_str(), m_PosX);
#endif
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

HitBox Entity::GetHurtBox() const {
    return {};
}

EntityState Entity::GetState() const {
    return m_State;
}

bool Entity::CanAttack(const Entity &e) const {
    const auto det_box_pos = ToWorldSpace(m_Stats.det_box);
    const auto pos = e.GetPosX();
    return m_State == EntityState::WALK &&
           (det_box_pos.low <= pos && pos <= det_box_pos.high);
}

float Entity::GetPosX() const {
    return m_PosX;
}

float Entity::SetPosX(float x) {
    return m_PosX = x;
}
