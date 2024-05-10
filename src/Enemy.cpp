#include "Enemy.hpp"
#include "DebugUtil/BattleLog.hpp"
#include <random>
#include "Sound.hpp"

Enemy::Enemy(const EnemyType type)
    : m_Type(type) {
    SetStats(EnemyStats::Stats[static_cast<size_t>(type)]);
}

void Enemy::StartAttack() {
    SetState(EntityState::ATTACK);
    m_AtkPrepTimer.Start();
#ifdef ENABLE_BATTLE_LOG
    printBattleLog("{} StarAttack at position: {}", m_Stats.name, m_PosX);
#endif
}

void Enemy::Draw(Util::Transform trans, Animation &anime) {
    trans.translation += glm::vec2{m_PosX, 0};
    trans.translation -= glm::vec2{anime.idle->GetScaledSize().x / 2.0f, 0.0f};
    float z = 1.0f;
    if (m_Type == EnemyType::ENEMY_TOWER) {
        z = -1.0f;
    }

    const auto state = GetState();
    if (state == EntityState::HITBACK) {
        trans.translation.y += hb_dy;
    }
    if (m_Type != EnemyType::ENEMY_TOWER && m_PrevDrawState != state) {
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

    const auto DrawImg = [this](AnimatedGameObject &img, Util::Transform &t) {
        const auto y = img.GetBottomRightPos().y;
        t.translation.y += (m_TargetY - y);
        img.GetDrawable()->Draw(t, 1.0f);
    };

    if (m_Type != EnemyType::ENEMY_TOWER) {
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

void Enemy::UpdateTimer(const double dt) {
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
    }
}

void Enemy::Walk(const float dt) {
    if (GetState() == EntityState::WALK) {
        m_PosX += float(m_Stats.speed) * dt;
    } else if (GetState() == EntityState::HITBACK) {
        m_PosX -= s_KnockbackSpeed * dt;
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

void Enemy::DealDamage(Entity &e) {
    e.GetHit(m_Stats.damage, *this);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dis(0.5);
    //Sounds::Attack1->Play();
    dis(gen) ? Sounds::Attack1->Play() : Sounds::Attack2->Play();
}

EnemyType Enemy::GetEnemyType() const {
    return m_Type;
}

bool Enemy::OnAttack() {
    const auto atk = m_OnAttack;
    m_OnAttack = false;
    return atk;
}

void Enemy::SetStatsModifier(float modifier) {
    assert(modifier >= 1.0f);
    m_Stats.damage *= modifier;
    m_Health *= modifier;
    m_FullHealth = m_Health;
    m_KnockBackHealth = m_Health / m_Stats.kb;
}

void Enemy::Attack() {
    if (GetState() == EntityState::HITBACK) {
        return;
    }
    assert(GetState() == EntityState::ATTACK);
    m_OnAttack = true;
    SetState(EntityState::IDLE);
    m_AtkCoolDownTimer.Start();
}

void Enemy::CoolDownComplete() {
    if (GetState() == EntityState::IDLE) {
        SetState(EntityState::WALK);
    }
}

HitBox Enemy::ToWorldSpace(HitBox hitbox) const {
    hitbox.high = m_PosX + hitbox.high;
    hitbox.low = m_PosX + hitbox.low;
    return hitbox;
}

void Enemy::SetY(float low, float high) {
    m_TargetY = s_Random.generate(low, high);
}

void Enemy::OnHitBack() {
    hb_vel_y = 140.0;
    land = 0;
}

Enemy::Animation EnemyAnimation::Tower() {
    Enemy::Animation a;

    a.idle =
        std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/stages/ec045_tw.png"});

    return a;
}

Enemy::Animation EnemyAnimation::Doge() {
    Enemy::Animation a;

    a.idle =
        std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/enemys/000/Animation/idle.png"});

    a.attack =
        std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/enemys/000/Animation/attack0.png",
            RESOURCE_DIR "/enemys/000/Animation/attack1.png",
            RESOURCE_DIR "/enemys/000/Animation/attack1.png"});
    a.attack->SetInterval(EnemyStats::Doge.atk_prep_time * 1000.0 / 2.0);

    a.walk =
        std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/enemys/000/Animation/walk0.png",
            RESOURCE_DIR "/enemys/000/Animation/walk1.png"});
    a.walk->SetInterval(300);
    a.walk->SetLooping(true);

    a.knockback =
        std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/enemys/000/Animation/knockback.png"});

    return a;
}
