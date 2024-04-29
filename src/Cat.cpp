#include "Cat.hpp"
#include "DebugUtil/BattleLog.hpp"
#include "Sound.hpp"
#include <random>

Cat::Cat(const CatType type, const int level)
    : m_Type(type) {
    SetStats(BaseCatStats::Stats[static_cast<size_t>(type)]);
    assert(level >= m_Stats.base_level);
    m_Health += (level - m_Stats.base_level) * m_Stats.health_diff;
    m_Stats.damage += (level - m_Stats.base_level) * m_Stats.damage_diff;
}

void Cat::StartAttack() {
    SetState(EntityState::ATTACK);
    m_AtkPrepTimer.Start();
#ifdef ENABLE_BATTLE_LOG
    printBattleLog("{} StarAttack at position: {:.2f}", m_Stats.name, m_PosX);
#endif
}

void Cat::Draw(Util::Transform trans, Animation &anime) {
    trans.translation += glm::vec2{m_PosX, 0};
    trans.translation += glm::vec2{anime.idle->GetScaledSize().x / 2.0f, 0};
    float z = 1.0f;
    if (m_Type == CatType::CAT_TOWER) {
        z = -1.0f;
    }

    const auto state = GetState();
    if (state == EntityState::HITBACK) {
        trans.translation.y += hb_dy;
    }
    if (m_Type != CatType::CAT_TOWER && m_PrevDrawState != state) {
        m_PrevDrawState = state;
        // restart the current state's animation
        switch (state) {
        case EntityState::WALK:
            anime.walk->Reset();
            anime.walk->Play();
            break;

        case EntityState::ATTACK:
            anime.attack->Reset();
            anime.attack->Play();
            break;

        case EntityState::IDLE:
            anime.idle->Reset();
            anime.idle->Play();
            break;

        case EntityState::HITBACK:
            break;
        }
    }

    const auto DrawImg = [this](AnimatedGameObject &img, Util::Transform& t) {
        const auto y = img.GetBottomRightPos().y;
        t.translation.y += (m_TargetY - y);
        img.GetDrawable()->Draw(t, 1.0f);
    };

    if (m_Type != CatType::CAT_TOWER) {
        switch (state) {
        case EntityState::WALK: 
            DrawImg(*anime.walk, trans);
            break;
        
        case EntityState::ATTACK: 
            DrawImg(*anime.attack, trans);
            break;
        
        case EntityState::IDLE: 
            if (anime.attack->IsPlaying()) {
                DrawImg(*anime.attack, trans);
            } else {
                DrawImg(*anime.idle, trans);
            }
            break;
        
        case EntityState::HITBACK:
            DrawImg(*anime.knockback, trans);
            break;
        }
    } else {
        anime.idle->Draw(trans, z, 0);
    }
}

void Cat::UpdateTimer(const double dt) {
    m_AtkPrepTimer.Update(dt);
    if (m_AtkPrepTimer.IsTimeOut()) {
        Attack();
    }

    m_AtkCoolDownTimer.Update(dt);
    if (m_AtkCoolDownTimer.IsTimeOut()) {
        CoolDownComplete();
    }

    m_KnockbackTimer.Update(dt);
    if (m_KnockbackTimer.IsTimeOut()) {
        SetState(EntityState::WALK);
        hb_dy = 0.0;
    }
}

void Cat::Walk(const double dt) {
    if (GetState() == EntityState::WALK) {
        m_PosX -= m_Stats.speed * dt;
    } else if (GetState() == EntityState::HITBACK) {
        m_PosX += s_KnockbackSpeed * dt;
        constexpr double gravity = -500.0;
        hb_vel_y += gravity * dt;
        hb_dy += hb_vel_y * dt;
        if (hb_dy <= 0 && hb_vel_y <= 0 && land == 0) {
            ++land;
            hb_vel_y = 110.0;
        }
        hb_dy = std::max(0.0, hb_dy);
    }
}

void Cat::DealDamage(Entity &e) {
    const auto attr = e.GetAttr();
    auto damage = m_Stats.damage;
    if (attr && std::find(m_Stats.strong.cbegin(), m_Stats.strong.cend(), *attr) != m_Stats.strong.cend()) {
        auto d = static_cast<double>(damage);
        d *= 1.5;
        damage = static_cast<int>(d);
    }
    e.GetHit(damage, *this);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dis(0.5);
    dis(gen) ? Sounds::Attack1->Play() : Sounds::Attack2->Play();
}

CatType Cat::GetCatType() const {
    return m_Type;
}

bool Cat::OnAttack() {
    const auto atk = m_OnAttack;
    m_OnAttack = false;
    return atk;
}

void Cat::ResetState() {
    SetState(EntityState::WALK);
    m_AtkCoolDownTimer.Stop();
    m_AtkPrepTimer.Stop();
    m_KnockbackTimer.Stop();
}

void Cat::Move(const float dx) {
    m_PosX += dx;
}

void Cat::SetY(float low, float high) {
    m_TargetY = s_Random.generate(low, high);
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
    hitbox.high = m_PosX + hitbox.high;
    hitbox.low = m_PosX + hitbox.low;
    return hitbox;
}

void Cat::OnHitBack() {
    hb_vel_y = 140.0;
    land = 0;
}