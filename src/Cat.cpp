#include "Cat.hpp"
#include "Enemy.hpp"
#include "DebugUtil/BattleLog.hpp"

Cat::Cat(const CatType type, std::function<void(Cat &)> atk_callback)
    : m_AtkCallback(atk_callback),
      m_Type(type) {
    assert(m_AtkCallback);
    SetCallbacks();
}

void Cat::StartAttack() {
    SetState(EntityState::ON_ATTACK);
    m_AtkPrepTimer.Start();
#ifdef ENABLE_BATTLE_LOG
    printBattleLog("{} StarAttack at position: {:.2f}", m_Stats.name, m_PosX);
#endif
}

void Cat::Draw(Util::Image &image) const {
    Util::Transform trans;
    trans.translation = {m_PosX, 0};
    image.Draw(trans, 1.0);
}

void Cat::Update() {
    Entity::OnUpdate();
}

void Cat::Walk(float dt) {
    if (GetState() == EntityState::WALK) {
        m_PosX -= m_Stats.speed * dt;
    } else if (GetState() == EntityState::KNOCK_BACK) {
        m_PosX += s_KnockbackSpeed * dt;
    }
}

void Cat::DealDamage(Entity &e) {
#ifdef ENABLE_BATTLE_LOG
    printBattleLog("{} deals damage {} to {}!", m_Stats.name, m_Stats.damage,
                   e.GetName());
#endif // ENABLE_BATTLE_LOG
    e.GetHit(m_Stats.damage, std::nullopt);
}

CatType Cat::GetCatType() const {
    return m_Type;
}

Cat::Cat(Cat &&other) noexcept
    : m_AtkCallback(other.m_AtkCallback),
      m_Type(other.m_Type) {
    SetStats(other.m_Stats);
    SetCallbacks();
}

Cat &Cat::operator=(Cat &&other) noexcept {
    m_Type = other.m_Type;
    SetStats(other.m_Stats);
    SetCallbacks();
    return *this;
}

void Cat::SetCallbacks() {
    m_AtkPrepTimer.SetTimeOutEvent([this] { Attack(); });
    m_AtkCoolDownTimer.SetTimeOutEvent([this] { CoolDownComplete(); });
}

void Cat::Attack() {
    if (GetState() == EntityState::KNOCK_BACK) {
        return;
    } 
    assert(GetState() == EntityState::ON_ATTACK);
    m_AtkCallback(*this);
    SetState(EntityState::ATTACK_COOLDOWN);
    m_AtkCoolDownTimer.Start();
}

void Cat::CoolDownComplete() {
    if (GetState() == EntityState::ATTACK_COOLDOWN) {
        SetState(EntityState::WALK);
    }
}

HitBox Cat::ToWorldSpace(HitBox hitbox) const {
    const auto len = hitbox.high - hitbox.low;
    hitbox.high = m_PosX;
    hitbox.low = m_PosX - len;
    return hitbox;
}
