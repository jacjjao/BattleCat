#include "Enemy.hpp"
#include "DebugUtil/BattleLog.hpp"

Enemy::Enemy(const EnemyType type)
    : m_Type(type) {
    SetStats(EnemyStats::Stats[static_cast<size_t>(type)]);
}

void Enemy::StartAttack() {
    SetState(EntityState::ATTACK);
    m_AtkPrepTimer.Start();
#ifdef ENABLE_BATTLE_LOG
    printBattleLog("{} StarAttack at position: {:.2f}", m_Stats.name, m_PosX);
#endif
}

void Enemy::Draw(Util::Image &image) const {
    Util::Transform trans;
    trans.translation = {m_PosX, 0};
    image.Draw(trans, 1.0f);
}

void Enemy::UpdateTimer(const double dt) {
    m_AtkPrepTimer.Update(dt);
    if (m_AtkPrepTimer.IsTimeOut()) {
        Attack();
    }

    m_AtkCoolDownTimer.Update(dt);
    if (m_AtkCoolDownTimer.IsTimeOut()) {
        CoolDownComplete();
    }

    m_KnockbackTimer.Update(dt);
    if (m_KnockbackTimer.IsTimeOut()) {
        SetState(EntityState::WALK);
    }
}

void Enemy::Walk(const float dt) {
    if (GetState() == EntityState::WALK) {
        m_PosX += m_Stats.speed * dt;
    } else if (GetState() == EntityState::HITBACK) {
        m_PosX -= s_KnockbackSpeed * dt;
    }
}

void Enemy::DealDamage(Entity &e) {
    e.GetHit(m_Stats.damage, *this);
}

EnemyType Enemy::GetEnemyType() const {
    return m_Type;
}

bool Enemy::OnAttack() {
    const auto atk = m_OnAttack;
    m_OnAttack = false;
    return atk;
}

void Enemy::SetStatsModifier(float modifier) {
    assert(modifier >= 1.0f);
    m_Stats.damage *= modifier;
    m_Health *= modifier;
    m_KnockBackHealth = m_Health / m_Stats.kb;
}

void Enemy::Attack() {
    if (GetState() == EntityState::HITBACK) {
        return;
    }
    assert(GetState() == EntityState::ATTACK);
    m_OnAttack = true;
    SetState(EntityState::IDLE);
    m_AtkCoolDownTimer.Start();
}

void Enemy::CoolDownComplete() {
    if (GetState() == EntityState::IDLE) {
        SetState(EntityState::WALK);
    }
}

HitBox Enemy::ToWorldSpace(HitBox hitbox) const {
    const auto len = hitbox.high - hitbox.low;
    hitbox.high = m_PosX + len;
    hitbox.low = m_PosX;
    return hitbox;
}
