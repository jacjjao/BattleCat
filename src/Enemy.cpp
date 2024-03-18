#include "Enemy.hpp"
#include "DebugUtil/BattleLog.hpp"

Enemy::Enemy(const EnemyType type)
    : m_Type(type) {
    SetStats(EnemyStats::Stats[static_cast<size_t>(type)]);
    SetCallbacks();
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

void Enemy::Update() {
    Entity::OnUpdate();
}

void Enemy::Walk(float dt) {
    if (GetState() == EntityState::WALK) {
        m_PosX += m_Stats.speed * dt;
    } else if (GetState() == EntityState::HITBACK) {
        m_PosX -= s_KnockbackSpeed * dt;
    }
}

void Enemy::DealDamage(Entity &e) {
#ifdef ENABLE_BATTLE_LOG
    printBattleLog("{} deals damage {} to {}!", m_Stats.name, m_Stats.damage,
                   e.GetName());
#endif // ENABLE_BATTLE_LOG
    e.GetHit(m_Stats.damage, m_Stats.attr);
}

EnemyType Enemy::GetEnemyType() const {
    return m_Type;
}

Enemy::Enemy(Enemy &&other) noexcept
    : m_Type(other.m_Type) {
    m_PosX = other.m_PosX;
    SetStats(other.m_Stats);
    SetCallbacks();
}

Enemy &Enemy::operator=(Enemy &&other) noexcept {
    m_Type = other.m_Type;
    m_PosX = other.m_PosX;
    SetStats(other.m_Stats);
    SetCallbacks();
    return *this;
}

bool Enemy::OnAttack() {
    const auto atk = m_OnAttack;
    m_OnAttack = false;
    return atk;
}

void Enemy::SetStatsModifier(float modifier) {
    assert(modifier >= 1.0f);
    m_Stats.damage *= modifier;
    m_Stats.health *= modifier;
    m_KnockBackHealth = m_Stats.health / m_Stats.kb;
}

void Enemy::SetCallbacks() {
    m_AtkPrepTimer.SetTimeOutEvent([this] { Attack(); });
    m_AtkCoolDownTimer.SetTimeOutEvent([this] { CoolDownComplete(); });
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
