#include "Enemy.hpp"
#include "DebugUtil/BattleLog.hpp"
#include <random>
#include "Sound.hpp"

Enemy::Enemy(const EnemyType type)
    : m_Type(type) {
    SetStats(EnemyStats::Stats[static_cast<size_t>(type)]);
}

void Enemy::StartAttack() {
    SetState(EntityState::ATTACK);
    m_AtkPrepTimer.Start();
#ifdef ENABLE_BATTLE_LOG
    printBattleLog("{} StarAttack at position: {}", m_Stats.name, m_PosX);
#endif
}

void Enemy::Draw(Util::Transform trans, Util::Image &image) const {
    trans.translation += glm::vec2{m_PosX, 0};
    trans.translation -= glm::vec2{image.GetSize().x / 2.0f, 0.0f};
    float z = 1.0f;
    if (m_Type == EnemyType::ENEMY_TOWER) {
        z = -1.0f;
    }
    image.Draw(trans, z);
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
        m_PosX += float(m_Stats.speed) * dt;
    } else if (GetState() == EntityState::HITBACK) {
        m_PosX -= s_KnockbackSpeed * dt;
    }
}

void Enemy::DealDamage(Entity &e) {
    e.GetHit(m_Stats.damage, *this);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dis(0.5);
    //Sounds::Attack1->Play();
    dis(gen) ? Sounds::Attack1->Play() : Sounds::Attack2->Play();
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
    m_FullHealth = m_Health;
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
    hitbox.high = m_PosX + hitbox.high;
    hitbox.low = m_PosX + hitbox.low;
    return hitbox;
}
