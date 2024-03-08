#include "Enemy.hpp"

void Enemy::Draw(Util::Image &image) const {
    Util::Transform trans;
    trans.translation = {m_PosX, 0};
    image.Draw(trans, 1.0f);
}

void Enemy::Update(const float dt) {
    if (m_State == EntityState::WALK) {
        m_PosX += m_Stats.speed * dt;
    }
}

HitBox Enemy::ToWorldSpace(HitBox hitbox) const {
    const auto len = hitbox.high - hitbox.low;
    hitbox.high = m_PosX + len;
    hitbox.low = m_PosX;
    return hitbox;
}
