#include "Enemy.hpp"

Enemy::Enemy(std::function<void(Enemy &)> atk_callback)
    : m_AtkCallback(atk_callback) {
    assert(m_AtkCallback);
    SetCallbacks();
}

void Enemy::Draw(Util::Image &image) const {
    Util::Transform trans;
    trans.translation = {m_PosX, 0};
    image.Draw(trans, 1.0f);
}

void Enemy::Update(const float dt) {
    if (GetState() == EntityState::WALK) {
        m_PosX += m_Stats.speed * dt;
    }
    Entity::OnUpdate();
}

void Enemy::DealDamage(Entity &e) {
#ifdef ENABLE_BATTLE_LOG
    printf("%s deals damage %d to %s!\n", m_Stats.name.c_str(), m_Stats.damage,
           e.GetName().c_str());
#endif // ENABLE_BATTLE_LOG
    e.GetHit(m_Stats.damage, std::nullopt);
}

Enemy::Enemy(Enemy &&other) noexcept
    : m_AtkCallback(other.m_AtkCallback) {
    SetStats(other.m_Stats);
    SetCallbacks();
}

Enemy &Enemy::operator=(Enemy &&other) noexcept {
    SetCallbacks();
    return *this;
}

void Enemy::SetCallbacks() {
    m_AtkPrepTimer.SetTimeOutEvent([this] { Attack(); });
    m_AtkCoolDownTimer.SetTimeOutEvent([this] { CoolDownComplete(); });
}

void Enemy::Attack() {
    m_AtkCallback(*this);
    SetState(EntityState::ATTACK_COOLDOWN);
    m_AtkCoolDownTimer.Start();
}

void Enemy::CoolDownComplete() {
    SetState(EntityState::WALK);
}

HitBox Enemy::ToWorldSpace(HitBox hitbox) const {
    const auto len = hitbox.high - hitbox.low;
    hitbox.high = m_PosX + len;
    hitbox.low = m_PosX;
    return hitbox;
}
