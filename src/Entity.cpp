#include "Entity.hpp"
#include "DebugUtil/BattleLog.hpp"

void Entity::StartAttack() {
    SetState(EntityState::ON_ATTACK);
    m_AtkPrepTimer.Start();
#ifdef ENABLE_BATTLE_LOG
    printBattleLog("%s StarAttack at position: %.2f\n", m_Stats.name.c_str(),
                   m_PosX);
#endif
}

void Entity::SetStats(const EntityStats &stats) {
    m_Stats = stats;
    m_AtkPrepTimer.SetTimeOutDur(m_Stats.atk_prep_time);
    m_AtkCoolDownTimer.SetTimeOutDur(m_Stats.atk_cool_down);
    m_KnockbackTimer.SetTimeOutDur(1.0); // tmp
    m_KnockbackTimer.SetTimeOutEvent([this] { ExitKnockbackState(); });
    m_KnockBackHealth = m_Stats.health / m_Stats.kb;
}

void Entity::GetHit(int damage, std::optional<EnemyAttr> attr) {
    m_Stats.health -= damage;
    m_TotalDamage += damage;
    if (m_TotalDamage >= m_KnockBackHealth) {
        SetState(EntityState::KNOCK_BACK);
        m_TotalDamage -= m_KnockBackHealth;
    }
}

int Entity::GetDamage(int damage, std::optional<EnemyAttr> attr) const {
    return damage;
}

std::optional<EnemyAttr> Entity::GetEnemyAttr() const {
    return m_Stats.attr;
}

HitBox Entity::GetHitBox() const {
    return ToWorldSpace(m_Stats.hit_box);
}

EntityState Entity::GetState() const {
    return m_State;
}

bool Entity::CanAttack(const Entity &e) const {
    const auto det_box_pos = ToWorldSpace(m_Stats.det_box);
    const auto pos = e.GetPosX();
    return m_State != EntityState::KNOCK_BACK &&
           (det_box_pos.low <= pos && pos <= det_box_pos.high);
}

float Entity::GetPosX() const {
    return m_PosX;
}

float Entity::SetPosX(float x) {
    return m_PosX = x;
}

bool Entity::IsSingleTarget() const {
    return m_Stats.single_target;
}

bool Entity::IsDead() const {
    return m_Stats.health == 0;
}

void Entity::SetState(EntityState state) {
#ifdef ENABLE_BATTLE_LOG
    printBattleLog("%s changed it's state from %s to %s\n",
                   m_Stats.name.c_str(), EntityStateToString(m_State).c_str(),
                   EntityStateToString(state).c_str());
#endif // ENABLE_BATTLE_LOG

    m_State = state;
    if (state == EntityState::KNOCK_BACK) {
        m_KnockbackTimer.Start();
    }
}

void Entity::OnUpdate() {
    m_AtkPrepTimer.Update();
    m_AtkCoolDownTimer.Update();
    m_KnockbackTimer.Update();
}

void Entity::ExitKnockbackState() {
    SetState(EntityState::WALK);
}
