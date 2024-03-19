#include "Cat.hpp"
#include "Enemy.hpp"
#include "DebugUtil/BattleLog.hpp"

Cat::Cat(const CatType type)
    : m_Type(type) {
    SetStats(BaseCatStats::Stats[static_cast<size_t>(type)]);
    SetCallbacks();
}

void Cat::StartAttack() {
    SetState(EntityState::ATTACK);
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
    } else if (GetState() == EntityState::HITBACK) {
        m_PosX += s_KnockbackSpeed * dt;
    }
}

void Cat::DealDamage(Entity &e) {
    e.GetHit(m_Stats.damage, std::nullopt);
#ifdef ENABLE_BATTLE_LOG
    printBattleLog("{} deals damage {} to {}!", m_Stats.name, m_Stats.damage,
                   e.GetName());
#endif // ENABLE_BATTLE_LOG
}

CatType Cat::GetCatType() const {
    return m_Type;
}

Cat::Cat(Cat &&other) noexcept
    : m_Type(other.m_Type) {
    m_PosX = other.m_PosX;
    SetStats(other.m_Stats);
    SetCallbacks();
}

Cat &Cat::operator=(Cat &&other) noexcept {
    m_Type = other.m_Type;
    m_PosX = other.m_PosX;
    SetStats(other.m_Stats);
    SetCallbacks();
    return *this;
}

bool Cat::OnAttack() {
    const auto atk = m_OnAttack;
    m_OnAttack = false;
    return atk;
}

void Cat::SetCallbacks() {
    m_AtkPrepTimer.SetTimeOutEvent([this] { Attack(); });
    m_AtkCoolDownTimer.SetTimeOutEvent([this] { CoolDownComplete(); });
}

void Cat::Attack() {
    if (GetState() == EntityState::HITBACK) {
        return;
    } 
    assert(GetState() == EntityState::ATTACK);
    m_OnAttack = true;
    SetState(EntityState::IDLE);
    m_AtkCoolDownTimer.Start();
}

void Cat::CoolDownComplete() {
    if (GetState() == EntityState::IDLE) {
        SetState(EntityState::WALK);
    }
}

HitBox Cat::ToWorldSpace(HitBox hitbox) const {
    const auto len = hitbox.high - hitbox.low;
    hitbox.high = m_PosX;
    hitbox.low = m_PosX - len;
    return hitbox;
}
