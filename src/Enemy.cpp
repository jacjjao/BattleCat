#include "Enemy.hpp"
#include "DebugUtil/BattleLog.hpp"
//#include <random>
#include "Sound.hpp"

Enemy::Enemy(const EnemyType type)
    : m_Type(type) {
    SetStats(EnemyStats::Stats[static_cast<size_t>(type)]);
    LoadResource();
    m_Anime.walk->Play();
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

    const auto DrawImg = [this](SharedRc::SharedAnimatedGameObject &img, Util::Transform &t) {
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
        m_Anime.idle->Draw(trans, z);
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
}

void Enemy::GetHit(int damage, const Entity &attacker) {
    m_Health -= damage;
    m_TotalDamage += damage;
    if (m_TotalDamage >= m_KnockBackHealth) {
        SetState(EntityState::HITBACK);
        m_TotalDamage %= m_KnockBackHealth;
        OnHitBack();
    }

    if(GetEnemyType() == EnemyType::ENEMY_TOWER){
        Sounds::AttackCastle->Play();
    }
    else {
        RandomFloatGenerator a;
        (a.generate(0.0f, 1.0f) < 0.5f) ? Sounds::Attack1->Play() : Sounds::Attack2->Play();
    }

#ifdef ENABLE_BATTLE_LOG
    printBattleLog("{} deals damage {} to {}! {} have {}hp left!",
                   attacker.GetName(), damage, GetName(), GetName(), m_Health);
#endif // ENABLE_BATTLE_LOG
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

    case EnemyType::DOGE:
        m_Anime = EnemyAnime::Doge();
        break;

    case EnemyType::SNACHE:
        m_Anime = EnemyAnime::Snache();
        break;

    case EnemyType::THOSE_GUYS:
        m_Anime = EnemyAnime::ThoseGuys();
        break;

    case EnemyType::HIPPOE:
        m_Anime = EnemyAnime::Hippoe();
        break;

    case EnemyType::PIGGE:
        m_Anime = EnemyAnime::Pigge();
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
        auto& tower = s_anime[static_cast<size_t>(EnemyType::ENEMY_TOWER)];
        tower.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR "/stages/ec045_tw.png"});
        tower.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR
                                               "/stages/ec045_tw.png"});
        tower.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR
                                               "/stages/ec045_tw.png"});
        tower.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR
                                               "/stages/ec045_tw.png"});
    }

    { 
        auto &doge = s_anime[static_cast<size_t>(EnemyType::DOGE)];

        doge.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/000/Animation/idle.png"});

        doge.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/000/Animation/walk0.png",
                RESOURCE_DIR "/enemys/000/Animation/walk1.png"});

        doge.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/000/Animation/attack0.png",
                RESOURCE_DIR "/enemys/000/Animation/attack1.png",
                RESOURCE_DIR "/enemys/000/Animation/attack1.png" // for padding
            });

        doge.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/000/Animation/knockback.png"});
    }

    {
        auto &snache = s_anime[static_cast<size_t>(EnemyType::SNACHE)];

        snache.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/001/Animation/idle.png"});

        snache.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/001/Animation/walk0.png",
                RESOURCE_DIR "/enemys/001/Animation/walk1.png"});

        snache.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/001/Animation/attack0.png",
                RESOURCE_DIR "/enemys/001/Animation/attack1.png",
                RESOURCE_DIR "/enemys/001/Animation/attack2.png",
                RESOURCE_DIR "/enemys/001/Animation/attack3.png",
                RESOURCE_DIR "/enemys/001/Animation/attack3.png" // for padding
            });

        snache.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/001/Animation/knockback.png"});
    }

    {
        auto &those = s_anime[static_cast<size_t>(EnemyType::THOSE_GUYS)];

        those.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/002/Animation/idle.png"});

        those.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/002/Animation/walk0.png",
                RESOURCE_DIR "/enemys/002/Animation/walk1.png",
                RESOURCE_DIR "/enemys/002/Animation/walk2.png",
                RESOURCE_DIR "/enemys/002/Animation/walk3.png",
                RESOURCE_DIR "/enemys/002/Animation/walk4.png",
                RESOURCE_DIR "/enemys/002/Animation/walk5.png"});

        those.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/002/Animation/attack0.png",
                RESOURCE_DIR "/enemys/002/Animation/attack1.png",
                RESOURCE_DIR "/enemys/002/Animation/attack2.png",
                RESOURCE_DIR "/enemys/002/Animation/attack3.png",
                RESOURCE_DIR "/enemys/002/Animation/attack4.png",
                RESOURCE_DIR "/enemys/002/Animation/attack5.png",
                RESOURCE_DIR "/enemys/002/Animation/attack6.png",
                RESOURCE_DIR "/enemys/002/Animation/attack7.png",
                RESOURCE_DIR "/enemys/002/Animation/attack7.png" // for padding
            });

        those.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR "/enemys/002/Animation/knockback.png"});
    }

    {
        auto &hippoe = s_anime[static_cast<size_t>(EnemyType::HIPPOE)];

        hippoe.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/003/Animation/idle.png"});

        hippoe.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/003/Animation/walk0.png",
                RESOURCE_DIR "/enemys/003/Animation/walk1.png"});

        hippoe.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/003/Animation/attack0.png",
                RESOURCE_DIR "/enemys/003/Animation/attack1.png",
                RESOURCE_DIR "/enemys/003/Animation/attack2.png",
                RESOURCE_DIR "/enemys/003/Animation/attack2.png" // for padding
            });

        hippoe.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/003/Animation/knockback.png"});
    }

    {
        auto &pigge = s_anime[static_cast<size_t>(EnemyType::PIGGE)];

        pigge.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/004/Animation/idle.png"});

        pigge.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/004/Animation/walk0.png",
                RESOURCE_DIR "/enemys/004/Animation/walk1.png",
                RESOURCE_DIR "/enemys/004/Animation/walk3.png"});

        pigge.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/004/Animation/attack0.png",
                RESOURCE_DIR "/enemys/004/Animation/attack1.png",
                RESOURCE_DIR "/enemys/004/Animation/attack2.png",
                RESOURCE_DIR "/enemys/004/Animation/attack3.png",
                RESOURCE_DIR "/enemys/004/Animation/attack4.png",
                RESOURCE_DIR "/enemys/004/Animation/attack5.png",
                RESOURCE_DIR "/enemys/004/Animation/attack5.png" // for padding
            });

        pigge.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/004/Animation/attack0.png"});
    }

    { 
        auto& jackie_peng =
            s_anime[static_cast<size_t>(EnemyType::JackiePeng)];
        jackie_peng.walk = std::make_unique<SharedRc::Animation>(
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
            });
        
        jackie_peng.attack = std::make_unique<SharedRc::Animation>(
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
                RESOURCE_DIR "/enemys/005/Animation/attack/attack8.png" // for padding
            });

        jackie_peng.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/005/Animation/idle.png"});

        jackie_peng.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/005/Animation/hitback.png"});
    }

    {
        auto& Gory =
            s_anime[static_cast<size_t>(EnemyType::GORY)];
        Gory.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/006/Animation/walk/walk0.png",
                RESOURCE_DIR "/enemys/006/Animation/walk/walk1.png",
                RESOURCE_DIR "/enemys/006/Animation/walk/walk2.png",
                RESOURCE_DIR "/enemys/006/Animation/walk/walk3.png",
                RESOURCE_DIR "/enemys/006/Animation/walk/walk4.png"
            });

        Gory.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/006/Animation/attack/attack0.png",
                RESOURCE_DIR "/enemys/006/Animation/attack/attack1.png",
                RESOURCE_DIR "/enemys/006/Animation/attack/attack2.png",
                RESOURCE_DIR "/enemys/006/Animation/attack/attack3.png",
                RESOURCE_DIR "/enemys/006/Animation/attack/attack4.png",
                RESOURCE_DIR "/enemys/006/Animation/attack/attack5.png",
                RESOURCE_DIR "/enemys/006/Animation/attack/attack6.png",
                RESOURCE_DIR "/enemys/006/Animation/attack/attack6.png"// for padding
            });

        Gory.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/006/Animation/idle.png"});

        Gory.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/006/Animation/hitback.png"});
    }

    {
        auto& BaaBaa =
            s_anime[static_cast<size_t>(EnemyType::BAABAA)];
        BaaBaa.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/007/Animation/walk/walk0.png",
                RESOURCE_DIR "/enemys/007/Animation/walk/walk1.png",
                RESOURCE_DIR "/enemys/007/Animation/walk/walk2.png",
                RESOURCE_DIR "/enemys/007/Animation/walk/walk3.png",
                RESOURCE_DIR "/enemys/007/Animation/walk/walk4.png"
            });

        BaaBaa.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/007/Animation/attack/attack0.png",
                RESOURCE_DIR "/enemys/007/Animation/attack/attack1.png",
                RESOURCE_DIR "/enemys/007/Animation/attack/attack2.png",
                RESOURCE_DIR "/enemys/007/Animation/attack/attack3.png",
                RESOURCE_DIR "/enemys/007/Animation/attack/attack4.png",
                RESOURCE_DIR "/enemys/007/Animation/attack/attack5.png",
                RESOURCE_DIR "/enemys/007/Animation/attack/attack6.png",
                RESOURCE_DIR "/enemys/007/Animation/attack/attack7.png",
                RESOURCE_DIR "/enemys/007/Animation/attack/attack8.png",
                RESOURCE_DIR "/enemys/007/Animation/attack/attack9.png",
                RESOURCE_DIR "/enemys/007/Animation/attack/attack10.png",
                RESOURCE_DIR "/enemys/007/Animation/attack/attack10.png"// for padding
            });

        BaaBaa.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/007/Animation/idle.png"});

        BaaBaa.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/007/Animation/hitback.png"});
    }

    {
        auto& SirSeal =
            s_anime[static_cast<size_t>(EnemyType::SIRSEAL)];
        SirSeal.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/008/Animation/walk/walk0.png",
                RESOURCE_DIR "/enemys/008/Animation/walk/walk1.png",
                RESOURCE_DIR "/enemys/008/Animation/walk/walk2.png",
                RESOURCE_DIR "/enemys/008/Animation/walk/walk3.png",
                RESOURCE_DIR "/enemys/008/Animation/walk/walk4.png"
            });

        SirSeal.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/008/Animation/attack/attack0.png",
                RESOURCE_DIR "/enemys/008/Animation/attack/attack1.png",
                RESOURCE_DIR "/enemys/008/Animation/attack/attack2.png",
                RESOURCE_DIR "/enemys/008/Animation/attack/attack3.png",
                RESOURCE_DIR "/enemys/008/Animation/attack/attack4.png",
                RESOURCE_DIR "/enemys/008/Animation/attack/attack5.png",
                RESOURCE_DIR "/enemys/008/Animation/attack/attack5.png"// for padding
            });

        SirSeal.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/008/Animation/idle.png"});

        SirSeal.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/008/Animation/hitback.png"});
    }

    {
        auto& Leboin =
            s_anime[static_cast<size_t>(EnemyType::LEBOIN)];
        Leboin.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/009/Animation/walk/walk0.png",
                RESOURCE_DIR "/enemys/009/Animation/walk/walk1.png",
                RESOURCE_DIR "/enemys/009/Animation/walk/walk2.png",
                RESOURCE_DIR "/enemys/009/Animation/walk/walk3.png"
            });

        Leboin.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/009/Animation/attack/attack0.png",
                RESOURCE_DIR "/enemys/009/Animation/attack/attack1.png",
                RESOURCE_DIR "/enemys/009/Animation/attack/attack2.png",
                RESOURCE_DIR "/enemys/009/Animation/attack/attack3.png",
                RESOURCE_DIR "/enemys/009/Animation/attack/attack4.png",
                RESOURCE_DIR "/enemys/009/Animation/attack/attack4.png"// for padding
            });

        Leboin.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/009/Animation/idle.png"});

        Leboin.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/009/Animation/hitback.png"});
    }

    {
        auto& KangRoo =
            s_anime[static_cast<size_t>(EnemyType::KANGROO)];
        KangRoo.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/010/Animation/walk/walk0.png",
                RESOURCE_DIR "/enemys/010/Animation/walk/walk1.png",
                RESOURCE_DIR "/enemys/010/Animation/walk/walk2.png",
                RESOURCE_DIR "/enemys/010/Animation/walk/walk3.png",
                RESOURCE_DIR "/enemys/010/Animation/walk/walk4.png",
                RESOURCE_DIR "/enemys/010/Animation/walk/walk5.png",
                RESOURCE_DIR "/enemys/010/Animation/walk/walk6.png"
            });

        KangRoo.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/010/Animation/attack/attack0.png",
                RESOURCE_DIR "/enemys/010/Animation/attack/attack1.png",
                RESOURCE_DIR "/enemys/010/Animation/attack/attack2.png",
                RESOURCE_DIR "/enemys/010/Animation/attack/attack3.png",
                RESOURCE_DIR "/enemys/010/Animation/attack/attack4.png",
                RESOURCE_DIR "/enemys/010/Animation/attack/attack5.png",
                RESOURCE_DIR "/enemys/010/Animation/attack/attack5.png"// for padding
            });

        KangRoo.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/010/Animation/idle.png"});

        KangRoo.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/010/Animation/hitback.png"});
    }

    {
        auto& OneHorn =
            s_anime[static_cast<size_t>(EnemyType::ONEHORN)];
        OneHorn.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/011/Animation/walk/walk0.png",
                RESOURCE_DIR "/enemys/011/Animation/walk/walk1.png",
                RESOURCE_DIR "/enemys/011/Animation/walk/walk2.png",
                RESOURCE_DIR "/enemys/011/Animation/walk/walk3.png",
                RESOURCE_DIR "/enemys/011/Animation/walk/walk4.png",
                RESOURCE_DIR "/enemys/011/Animation/walk/walk5.png",
                RESOURCE_DIR "/enemys/011/Animation/walk/walk6.png",
                RESOURCE_DIR "/enemys/011/Animation/walk/walk7.png",
                RESOURCE_DIR "/enemys/011/Animation/walk/8.png"
            });

        OneHorn.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/011/Animation/attack/attack0.png",
                RESOURCE_DIR "/enemys/011/Animation/attack/attack1.png",
                RESOURCE_DIR "/enemys/011/Animation/attack/attack2.png",
                RESOURCE_DIR "/enemys/011/Animation/attack/attack3.png",
                RESOURCE_DIR "/enemys/011/Animation/attack/attack4.png",
                RESOURCE_DIR "/enemys/011/Animation/attack/attack4.png"// for padding
            });

        OneHorn.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/011/Animation/idle.png"});

        OneHorn.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/011/Animation/hitback.png"});
    }
}

const Enemy::Animation EnemyAnimeResource::Get(const EnemyType type) {
    if (!s_init) {
        Init();
        s_init = true;
    }
    const auto &anime = s_anime[static_cast<size_t>(type)];

    Enemy::Animation obj; 
    obj.idle =
        std::make_unique<SharedRc::SharedAnimatedGameObject>(*(anime.idle));
    obj.attack =
        std::make_unique<SharedRc::SharedAnimatedGameObject>(*(anime.attack));
    obj.walk =
        std::make_unique<SharedRc::SharedAnimatedGameObject>(*(anime.walk));
    obj.knockback = std::make_unique<SharedRc::SharedAnimatedGameObject>(
        *(anime.knockback));

    return obj;
}
