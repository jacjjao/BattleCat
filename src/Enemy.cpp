#include "Enemy.hpp"
#include "DebugUtil/BattleLog.hpp"
#include <random>
#include "Sound.hpp"

Enemy::Enemy(const EnemyType type)
    : m_Type(type) {
    SetStats(EnemyStats::Stats[static_cast<size_t>(type)]);
    LoadResource();
}

void Enemy::StartAttack() {
    SetState(EntityState::ATTACK);
    m_AtkPrepTimer.Start();
#ifdef ENABLE_BATTLE_LOG
    printBattleLog("{} StarAttack at position: {}", m_Stats.name, m_PosX);
#endif
}

void Enemy::Draw(Util::Transform trans) {
    trans.translation += glm::vec2{m_PosX, 0};
    trans.translation -= glm::vec2{m_Anime.idle->GetScaledSize().x / 2.0f, 0.0f};
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
            m_Anime.walk->Reset();
            m_Anime.walk->Play();
            break;

        case EntityState::ATTACK:
            m_Anime.attack->Reset();
            m_Anime.attack->Play();
            break;

        case EntityState::IDLE:
            m_Anime.idle->Reset();
            m_Anime.idle->Play();
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
            DrawImg(*m_Anime.walk, trans);
            break;

        case EntityState::ATTACK:
            DrawImg(*m_Anime.attack, trans);
            break;

        case EntityState::IDLE:
            if (m_Anime.attack->IsPlaying()) {
                DrawImg(*m_Anime.attack, trans);
            } else {
                DrawImg(*m_Anime.idle, trans);
            }
            break;

        case EntityState::HITBACK:
            DrawImg(*m_Anime.knockback, trans);
            break;
        }
    } else {
        m_Anime.idle->Draw(trans, z, 0);
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

void Enemy::LoadResource() {
    switch (m_Type) { 
    case EnemyType::ENEMY_TOWER:
        m_Anime = EnemyAnime::Tower();
        break;

    case EnemyType::JackiePeng:
        m_Anime = EnemyAnime::JackiePeng();
        break;    

    default:
        throw std::runtime_error{"Resource does not available"};
    }
}

void EnemyAnimeResource::Init() {
    s_anime.resize(static_cast<size_t>(EnemyType::ENEMY_TYPE_COUNT));
    
    { 
        auto& jackie_peng =
            s_anime[static_cast<size_t>(EnemyType::JackiePeng)];
        jackie_peng.walk = std::make_shared<Util::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/005/Animation/walk/walk0.png",
                RESOURCE_DIR "/enemys/005/Animation/walk/walk1.png",
                RESOURCE_DIR "/enemys/005/Animation/walk/walk2.png",
                RESOURCE_DIR "/enemys/005/Animation/walk/walk3.png",
                RESOURCE_DIR "/enemys/005/Animation/walk/walk4.png",
                RESOURCE_DIR "/enemys/005/Animation/walk/walk5.png",
                RESOURCE_DIR "/enemys/005/Animation/walk/walk6.png",
                RESOURCE_DIR "/enemys/005/Animation/walk/walk7.png",
                RESOURCE_DIR "/enemys/005/Animation/walk/walk8.png",
                RESOURCE_DIR "/enemys/005/Animation/walk/walk9.png",
                RESOURCE_DIR "/enemys/005/Animation/walk/walk10.png",
                RESOURCE_DIR "/enemys/005/Animation/walk/walk11.png",
            },
            false, 100);
        
        jackie_peng.attack = std::make_shared<Util::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/005/Animation/attack/attack0.png",
                RESOURCE_DIR "/enemys/005/Animation/attack/attack1.png",
                RESOURCE_DIR "/enemys/005/Animation/attack/attack2.png",
                RESOURCE_DIR "/enemys/005/Animation/attack/attack3.png",
                RESOURCE_DIR "/enemys/005/Animation/attack/attack4.png",
                RESOURCE_DIR "/enemys/005/Animation/attack/attack5.png",
                RESOURCE_DIR "/enemys/005/Animation/attack/attack6.png",
                RESOURCE_DIR "/enemys/005/Animation/attack/attack7.png",
                RESOURCE_DIR "/enemys/005/Animation/attack/attack8.png",
                RESOURCE_DIR
                "/enemys/005/Animation/attack/attack8.png" // for padding
            },
            false, 100);

        jackie_peng.idle = std::make_shared<Util::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/005/Animation/idle.png"},
            false, 100);

        jackie_peng.knockback = std::make_shared<Util::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/005/Animation/hitback.png"},
            false, 100);
    }
}

const EnemyAnimeResource::UtilAnime& EnemyAnimeResource::Get(const EnemyType type) {
    if (!s_init) {
        Init();
        s_init = true;
    }
    return s_anime[static_cast<size_t>(type)];
}
