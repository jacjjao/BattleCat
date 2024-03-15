#include "Enemy.hpp"
#include "DebugUtil/BattleLog.hpp"

Enemy::Enemy(const EnemyType type, float pos,
             std::function<void(Enemy &)> atk_callback)
    : m_AtkCallback(atk_callback),
      m_Type(type) {
    assert(m_AtkCallback);
    m_PosX = pos;
    SetStats(EnemyStats::Stats[static_cast<size_t>(type)]);
    SetCallbacks();
}

void Enemy::StartAttack() {
    SetState(EntityState::ON_ATTACK);
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
    } else if (GetState() == EntityState::KNOCK_BACK) {
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
    : m_AtkCallback(other.m_AtkCallback),
      m_Type(other.m_Type) {
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

void Enemy::SetCallbacks() {
    m_AtkPrepTimer.SetTimeOutEvent([this] { Attack(); });
    m_AtkCoolDownTimer.SetTimeOutEvent([this] { CoolDownComplete(); });
}

void Enemy::Attack() {
    if (GetState() == EntityState::KNOCK_BACK) {
        return;
    }
    assert(GetState() == EntityState::ON_ATTACK);
    m_AtkCallback(*this);
    SetState(EntityState::ATTACK_COOLDOWN);
    m_AtkCoolDownTimer.Start();
}

void Enemy::CoolDownComplete() {
    if (GetState() == EntityState::ATTACK_COOLDOWN) {
        SetState(EntityState::WALK);
    }
}

HitBox Enemy::ToWorldSpace(HitBox hitbox) const {
    const auto len = hitbox.high - hitbox.low;
    hitbox.high = m_PosX + len;
    hitbox.low = m_PosX;
    return hitbox;
}
