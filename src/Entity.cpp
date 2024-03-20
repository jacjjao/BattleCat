#include "Entity.hpp"
#include "DebugUtil/BattleLog.hpp"

void Entity::SetStats(const EntityStats &stats) {
    m_Stats = stats;
    m_AtkPrepTimer.SetTimeOutDur(m_Stats.atk_prep_time);
    m_AtkCoolDownTimer.SetTimeOutDur(m_Stats.atk_cool_down);
    m_KnockbackTimer.SetTimeOutDur(s_KnockbackDuration); 
    m_KnockBackHealth = m_Stats.health / m_Stats.kb;
}

void Entity::GetHit(int damage, const Entity &attacker) {
    const auto attr = attacker.GetAttr();
    if (attr && std::find(m_Stats.strong.cbegin(), m_Stats.strong.cend(),
                          *attr) != m_Stats.strong.cend()) {
        auto d = static_cast<double>(damage);
        d *= 0.5;
        damage = static_cast<int>(d);
    }
    m_Stats.health -= damage;
    m_TotalDamage += damage;
    if (m_TotalDamage >= m_KnockBackHealth) {
        SetState(EntityState::HITBACK);
        m_TotalDamage %= m_KnockBackHealth;
    }

#ifdef ENABLE_BATTLE_LOG
    printBattleLog("{} deals damage {} to {}!", attacker.GetName(), damage,
                   GetName());
#endif // ENABLE_BATTLE_LOG
}

HitBox Entity::GetHitBox() const {
    return ToWorldSpace(m_Stats.hit_box);
}

EntityState Entity::GetState() const {
    return m_State;
}

bool Entity::IsInRange(const Entity &e) const {
    const auto det_box_pos = ToWorldSpace(m_Stats.det_box);
    const auto pos = e.GetPosX();
    return e.GetState() != EntityState::HITBACK &&
           (det_box_pos.low <= pos && pos <= det_box_pos.high);
}

float Entity::GetPosX() const {
    return m_PosX;
}

void Entity::SetPosX(float pos) {
    m_PosX = pos;
}

bool Entity::IsSingleTarget() const {
    return m_Stats.single_target;
}

bool Entity::IsDead() const {
    return m_Stats.health <= 0;
}

std::optional<EnemyAttr> Entity::GetAttr() const {
    return m_Stats.attr;
}

void Entity::SetState(EntityState state) {
#ifdef ENABLE_BATTLE_LOG
    printBattleLog("{} changed it's state from {} to {}", m_Stats.name,
                   EntityStateToString(m_State), EntityStateToString(state));
#endif // ENABLE_BATTLE_LOG

    m_State = state;
    if (state == EntityState::HITBACK) {
        m_KnockbackTimer.Start();
    }
}
