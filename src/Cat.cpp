#include "Cat.hpp"

void Cat::Draw(Util::Image &image) const {
    Util::Transform trans;
    trans.translation = {m_PosX, 0};
    image.Draw(trans, 1.0);
}

void Cat::Update(float dt) {
    m_PosX -= m_Stats.speed * dt;
}
