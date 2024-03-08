#include "Cat.hpp"
#include "Enemy.hpp"

Cat::Cat(std::function<void(Cat &)> atk_callback)
    : m_AtkCallback(atk_callback) {
    assert(m_AtkCallback);
    m_AtkPrepTimer.SetTimeOutEvent([this] { Attack(); });
}

void Cat::Draw(Util::Image &image) const {
    Util::Transform trans;
    trans.translation = {m_PosX, 0};
    image.Draw(trans, 1.0);
}

void Cat::Update(float dt) {
    Entity::OnUpdate();
    if (m_State == EntityState::WALK) {
        m_PosX -= m_Stats.speed * dt;
    }
}

void Cat::DealDamage(Entity &e) {
#ifdef ENABLE_BATTLE_LOG
    printf("%s deals damage %d to %s!\n", m_Stats.name.c_str(), m_Stats.damage,
           e.GetName().c_str());
#endif // ENABLE_BATTLE_LOG

    e.GetHit(m_Stats.damage, std::nullopt);
}

void Cat::Attack() {
    m_AtkCallback(*this);
}

HitBox Cat::ToWorldSpace(HitBox hitbox) const {
    const auto len = hitbox.high - hitbox.low;
    hitbox.high = m_PosX;
    hitbox.low = m_PosX - len;
    return hitbox;
}
