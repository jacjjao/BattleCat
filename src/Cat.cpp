#include "Cat.hpp"
#include "Enemy.hpp"

Cat::Cat(std::function<void(Cat &)> atk_callback)
    : m_AtkCallback(atk_callback) {
    assert(m_AtkCallback);
    SetCallbacks();
}

void Cat::Draw(Util::Image &image) const {
    Util::Transform trans;
    trans.translation = {m_PosX, 0};
    image.Draw(trans, 1.0);
}

void Cat::Update(float dt) {
    if (GetState() == EntityState::WALK) {
        m_PosX -= m_Stats.speed * dt;
    }
    Entity::OnUpdate();
}

void Cat::DealDamage(Entity &e) {
#ifdef ENABLE_BATTLE_LOG
    printf("%s deals damage %d to %s!\n", m_Stats.name.c_str(), m_Stats.damage,
           e.GetName().c_str());
#endif // ENABLE_BATTLE_LOG
    e.GetHit(m_Stats.damage, std::nullopt);
}

Cat::Cat(Cat &&other) noexcept : m_AtkCallback(other.m_AtkCallback) {
    SetStats(other.m_Stats);
    SetCallbacks();
}

Cat &Cat::operator=(Cat &&other) noexcept {
    SetCallbacks();
    return *this;
}

void Cat::SetCallbacks() {
    m_AtkPrepTimer.SetTimeOutEvent([this] { Attack(); });
    m_AtkCoolDownTimer.SetTimeOutEvent([this] { CoolDownComplete(); });
}

void Cat::Attack() {
    m_AtkCallback(*this);
    SetState(EntityState::ATTACK_COOLDOWN);
    m_AtkCoolDownTimer.Start();
}

void Cat::CoolDownComplete() {
    SetState(EntityState::WALK);
}

HitBox Cat::ToWorldSpace(HitBox hitbox) const {
    const auto len = hitbox.high - hitbox.low;
    hitbox.high = m_PosX;
    hitbox.low = m_PosX - len;
    return hitbox;
}
