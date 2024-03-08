#include "Enemy.hpp"

void Enemy::Draw(Util::Image &image) const {
    Util::Transform trans;
    trans.translation = {m_PosX, 0};
    image.Draw(trans, 1.0f);
}

void Enemy::Update(const float dt) {
    m_PosX += m_Stats.speed * dt;
}
