#include "BattleBackground.hpp"

BattleBackground::BattleBackground(const std::string &path) : m_Img(path) {}

void BattleBackground::SetScaleX(const float x) {
    m_Trans.scale.x = x;
}

void BattleBackground::Draw(const Camera &cam) {
    auto t = m_Trans;
    t.translation.x += cam.GetTransform().translation.x;
    m_Img.Draw(t, -10.0f);
}

void BattleBackground::ConstraintCam(Camera &cam) const {
    const float w = (m_Img.GetSize().x * 0.5f) * m_Trans.scale.x;
    constexpr int sw = 1280 / 2;

    auto ct = cam.GetTransform();
    if (ct.translation.x + sw > w) {
        ct.translation.x = w - sw;
    } else if (ct.translation.x - sw < -w) {
        ct.translation.x = -w + sw;
    }
    cam.SetTransform(ct);
}
