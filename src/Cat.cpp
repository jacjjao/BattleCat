#include "Cat.hpp"
#include "DebugUtil/BattleLog.hpp"
#include "Sound.hpp"
//#include <random>

Cat::Cat(const CatType type, const int level)
    : m_Type(type) {
    SetStats(BaseCatStats::Stats[static_cast<size_t>(type)]);
    assert(level >= m_Stats.base_level);
    m_Health += (level - m_Stats.base_level) * m_Stats.health_diff;
    m_KnockBackHealth = m_Health / m_Stats.kb;
    m_Stats.damage += (level - m_Stats.base_level) * m_Stats.damage_diff;
    LoadResource();
    m_Anime.walk->Play();
}

void Cat::StartAttack() {
    SetState(EntityState::ATTACK);
    m_AtkPrepTimer.Start();
#ifdef ENABLE_BATTLE_LOG
    printBattleLog("{} StarAttack at position: {:.2f}", m_Stats.name, m_PosX);
#endif
}

void Cat::Draw(Util::Transform trans) {
    trans.translation += glm::vec2{m_PosX, 0};
    trans.translation += glm::vec2{m_Anime.idle->GetScaledSize().x / 2.0f, 0};
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

    const auto DrawImg = [this](SharedRc::SharedAnimatedGameObject &img, Util::Transform& t) {
        const auto y = img.GetBottomRightPos().y;
        t.translation.y += (m_TargetY - y);
        img.GetDrawable()->Draw(t, 1.0f);
    };

    if (m_Type != CatType::CAT_TOWER) {
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

    if(m_HitParticle->IsPlaying()){
        m_HitParticle->Draw(trans,z+1);
    }
    else{
        m_HitParticle->Reset();
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
}

void Cat::GetHit(int damage, const Entity &attacker) {
    const auto attr = attacker.GetAttr();
    if (attr && std::find(m_Stats.strong.cbegin(), m_Stats.strong.cend(),
                          *attr) != m_Stats.strong.cend()) {
        auto d = static_cast<double>(damage);
        d *= 0.5;
        damage = static_cast<int>(d);
    }
    m_Health -= damage;
    m_TotalDamage += damage;
    if (m_TotalDamage >= m_KnockBackHealth) {
        SetState(EntityState::HITBACK);
        m_TotalDamage %= m_KnockBackHealth;
        OnHitBack();
    }

    if(GetCatType() == CatType::CAT_TOWER){
        Sounds::AttackCastle->Play();
    }
    else {
        RandomFloatGenerator a;
        (a.generate(0.0f, 1.0f) < 0.5f) ? Sounds::Attack1->Play() : Sounds::Attack2->Play();
    }

    m_HitParticle->Play();

#ifdef ENABLE_BATTLE_LOG
    printBattleLog("{} deals damage {} to {}! {} have {}hp left!",
                   attacker.GetName(), damage, GetName(), GetName(), m_Health);
#endif // ENABLE_BATTLE_LOG
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

void Cat::LoadResource() {
    switch (m_Type) { 
    case CatType::CAT_TOWER:
        m_Anime = CatAnime::Tower();
        break;

    case CatType::CAT:
        m_Anime = CatAnime::Cat();
        break;

    case CatType::TANK_CAT:
        m_Anime = CatAnime::Tank();
        break;

    case CatType::AXE_CAT:
        m_Anime = CatAnime::Axe();
        break;

    case CatType::CRAZED_GROSS_CAT:
        m_Anime = CatAnime::Gross();
        break;

    case CatType::COW_CAT:
        m_Anime = CatAnime::Cow();
        break;

    case CatType::BIRD_CAT:
        m_Anime = CatAnime::Bird();
        break;

    case CatType::FISH_CAT:
        m_Anime = CatAnime::Fish();
        break;

    case CatType::LIZARD_CAT:
        m_Anime = CatAnime::Lizard();
        break;

    case CatType::TITAN_CAT:
        m_Anime = CatAnime::Titan();
        break;

    case CatType::ACTRESS_CAT:
        m_Anime = CatAnime::Actress();
        break;

    case CatType::KUNG_FU_CAT:
        m_Anime = CatAnime::KungFu();
        break;

    case CatType::MR_CAT:
        m_Anime = CatAnime::Mr();
        break;

    case CatType::BONDAGE_CAT:
        m_Anime = CatAnime::Bondage();
        break;

    case CatType::DOM_CAT:
        m_Anime = CatAnime::Dom();
        break;

    case CatType::CATS_IN_A_BOX:
        m_Anime = CatAnime::CatInBox();
        break;

    case CatType::PANTIES_CAT:
        m_Anime = CatAnime::Panties();
        break;

    case CatType::MONEKO:
        m_Anime = CatAnime::Moneko();
        break;

    default:
        throw std::runtime_error{"Unavailable cat resource"};
    }

    m_HitParticle = std::make_unique<SharedRc::SharedAnimatedGameObject>(*(CatAnimeResource::s_HitParticle));
    m_HitParticle->SetInterval(67);
    m_HitParticle->SetLooping(false);
}

void CatAnimeResource::Init() {
    s_anime.resize(static_cast<size_t>(CatType::CAT_TYPE_COUNT));

    s_HitParticle = std::make_unique<SharedRc::Animation>(std::initializer_list<std::string>{
        RESOURCE_DIR"/img/hit/hit0.png",
        RESOURCE_DIR"/img/hit/hit1.png",
        RESOURCE_DIR"/img/hit/hit2.png",
        RESOURCE_DIR"/img/hit/hit3.png",
        RESOURCE_DIR"/img/hit/hit4.png",
        RESOURCE_DIR"/img/hit/hit5.png",
        RESOURCE_DIR"/img/hit/hit6.png",
        RESOURCE_DIR"/img/hit/hit7.png",
        RESOURCE_DIR"/img/hit/hit8.png",
        RESOURCE_DIR"/img/hit/hit9.png",
        RESOURCE_DIR"/img/hit/hit10.png"
    });

    {
        auto &tower = s_anime[static_cast<size_t>(CatType::CAT_TOWER)];
        tower.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR
                                               "/cats/nyankocastle.png"});
        tower.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR
                                               "/cats/nyankocastle.png"});
        tower.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR
                                               "/cats/nyankocastle.png"});
        tower.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR
                                               "/cats/nyankocastle.png"});
    }

    {
        auto &cat = s_anime[static_cast<size_t>(CatType::CAT)];

        cat.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR "/cats/000/Animation/idle.png"});

        cat.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/000/Animation/walk0.png",
                RESOURCE_DIR "/cats/000/Animation/walk1.png"});

        cat.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/000/Animation/attack_prev0.png",
                RESOURCE_DIR "/cats/000/Animation/attack_prev1.png",
                RESOURCE_DIR "/cats/000/Animation/attack_post.png",
                RESOURCE_DIR "/cats/000/Animation/attack_post.png" // for padding
            });

        cat.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/000/Animation/knockback.png"});
    }

    {
        auto &tank = s_anime[static_cast<size_t>(CatType::TANK_CAT)];

        tank.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR
                                               "/cats/001/Animation/idle.png"});

        tank.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/001/Animation/walk0.png",
                RESOURCE_DIR "/cats/001/Animation/walk1.png"});

        tank.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/001/Animation/attack0.png",
                RESOURCE_DIR "/cats/001/Animation/attack1.png",
                RESOURCE_DIR "/cats/001/Animation/attack2.png",
                RESOURCE_DIR "/cats/001/Animation/attack2.png" // for padding
            });

        tank.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/001/Animation/knockback.png"});
    }

    {
        auto &axe = s_anime[static_cast<size_t>(CatType::AXE_CAT)];

        axe.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR
                                               "/cats/002/Animation/idle.png"});

        axe.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/002/Animation/walk0.png",
                RESOURCE_DIR "/cats/002/Animation/walk1.png"});

        axe.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/002/Animation/attack0.png",
                RESOURCE_DIR "/cats/002/Animation/attack1.png",
                RESOURCE_DIR "/cats/002/Animation/attack2.png",
                RESOURCE_DIR "/cats/002/Animation/attack3.png",
                RESOURCE_DIR "/cats/002/Animation/attack3.png" // for padding
            });

        axe.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/002/Animation/knockback.png"});
    }

    {
        auto &gross = s_anime[static_cast<size_t>(CatType::CRAZED_GROSS_CAT)];

        gross.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR "/cats/003/Animation/idle.png"});

        gross.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/003/Animation/walk0.png",
                RESOURCE_DIR "/cats/003/Animation/walk1.png",
                RESOURCE_DIR "/cats/003/Animation/walk2.png",
                RESOURCE_DIR "/cats/003/Animation/walk3.png",
                RESOURCE_DIR "/cats/003/Animation/walk4.png"});

        gross.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/003/Animation/attack0.png",
                RESOURCE_DIR "/cats/003/Animation/attack1.png",
                RESOURCE_DIR "/cats/003/Animation/attack2.png",
                RESOURCE_DIR "/cats/003/Animation/attack2.png" // for padding
            });

        gross.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR "/cats/003/Animation/idle.png"});
    }

    {
        auto &cow = s_anime[static_cast<size_t>(CatType::COW_CAT)];

        cow.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR
                                               "/cats/004/Animation/idle.png"});

        cow.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/004/Animation/walk0.png",
                RESOURCE_DIR "/cats/004/Animation/walk1.png",
                RESOURCE_DIR "/cats/004/Animation/walk2.png",
                RESOURCE_DIR "/cats/004/Animation/walk3.png"});

        cow.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/004/Animation/attack0.png",
                RESOURCE_DIR "/cats/004/Animation/attack1.png",
                RESOURCE_DIR "/cats/004/Animation/attack1.png" // for padding
            });

        cow.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/004/Animation/knockback.png"});
    }

    {
        auto &bird = s_anime[static_cast<size_t>(CatType::BIRD_CAT)];

        bird.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/005/Animation/walk0.png",
                RESOURCE_DIR "/cats/005/Animation/walk1.png",
                RESOURCE_DIR "/cats/005/Animation/walk2.png"});

        bird.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/005/Animation/walk0.png",
                RESOURCE_DIR "/cats/005/Animation/walk1.png",
                RESOURCE_DIR "/cats/005/Animation/walk2.png"});

        bird.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/005/Animation/attack0.png",
                RESOURCE_DIR "/cats/005/Animation/attack1.png",
                RESOURCE_DIR "/cats/005/Animation/attack2.png",
                RESOURCE_DIR "/cats/005/Animation/attack3.png",
                RESOURCE_DIR "/cats/005/Animation/attack3.png" // for padding
            });

        bird.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/005/Animation/knockback.png"});
    }

    {
        auto &fish = s_anime[static_cast<size_t>(CatType::FISH_CAT)];

        fish.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/006/Animation/idle.png",
            });

        fish.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/006/Animation/idle.png",
                RESOURCE_DIR "/cats/006/Animation/walk0.png",
                RESOURCE_DIR "/cats/006/Animation/walk1.png",
                RESOURCE_DIR "/cats/006/Animation/walk2.png",
                RESOURCE_DIR "/cats/006/Animation/walk3.png",
                RESOURCE_DIR "/cats/006/Animation/walk4.png",
                RESOURCE_DIR "/cats/006/Animation/walk5.png",
                RESOURCE_DIR "/cats/006/Animation/walk6.png"});

        fish.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/006/Animation/attack0.png",
                RESOURCE_DIR "/cats/006/Animation/attack1.png",
                RESOURCE_DIR "/cats/006/Animation/attack2.png",
                RESOURCE_DIR "/cats/006/Animation/attack2.png" // for padding
            });

        fish.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/006/Animation/attack0.png"});
    }

    {
        auto &lizard = s_anime[static_cast<size_t>(CatType::LIZARD_CAT)];

        lizard.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/007/Animation/idle.png",
            });

        lizard.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/007/Animation/idle.png",
                RESOURCE_DIR "/cats/007/Animation/walk0.png",
                RESOURCE_DIR "/cats/007/Animation/walk1.png",
                RESOURCE_DIR "/cats/007/Animation/walk2.png",
                RESOURCE_DIR "/cats/007/Animation/walk3.png"});

        lizard.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/007/Animation/attack0.png",
                RESOURCE_DIR "/cats/007/Animation/attack1.png",
                RESOURCE_DIR "/cats/007/Animation/attack2.png",
                RESOURCE_DIR "/cats/007/Animation/attack3.png",
                RESOURCE_DIR "/cats/007/Animation/attack4.png",
                RESOURCE_DIR "/cats/007/Animation/attack4.png" // for padding
            });

        lizard.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/007/Animation/attack0.png"});
    }

    {
        auto &titan = s_anime[static_cast<size_t>(CatType::TITAN_CAT)];

        titan.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/008/Animation/idle.png",
            });

        titan.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/008/Animation/walk0.png",
                RESOURCE_DIR "/cats/008/Animation/walk1.png",
                RESOURCE_DIR "/cats/008/Animation/walk2.png",
                RESOURCE_DIR "/cats/008/Animation/walk3.png",
                RESOURCE_DIR "/cats/008/Animation/walk4.png",
                RESOURCE_DIR "/cats/008/Animation/walk5.png",
                RESOURCE_DIR "/cats/008/Animation/walk6.png",
            });

        titan.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/008/Animation/attack0.png",
                RESOURCE_DIR "/cats/008/Animation/attack1.png",
                RESOURCE_DIR "/cats/008/Animation/attack2.png",
                RESOURCE_DIR "/cats/008/Animation/attack3.png",
                RESOURCE_DIR "/cats/008/Animation/attack4.png",
                RESOURCE_DIR "/cats/008/Animation/attack5.png",
                RESOURCE_DIR "/cats/008/Animation/attack6.png",
                RESOURCE_DIR "/cats/008/Animation/attack6.png" // for padding
            });

        titan.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/008/Animation/hitback.png"});
    }

    {
        auto &actress = s_anime[static_cast<size_t>(CatType::ACTRESS_CAT)];

        actress.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/009/Animation/idle.png",
            });

        actress.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/009/Animation/walk0.png",
                RESOURCE_DIR "/cats/009/Animation/walk1.png",
                RESOURCE_DIR "/cats/009/Animation/walk2.png",
                RESOURCE_DIR "/cats/009/Animation/walk3.png",
                RESOURCE_DIR "/cats/009/Animation/walk4.png"
            });

        actress.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/009/Animation/attack0.png",
                RESOURCE_DIR "/cats/009/Animation/attack1.png",
                RESOURCE_DIR "/cats/009/Animation/attack2.png",
                RESOURCE_DIR "/cats/009/Animation/attack3.png",
                RESOURCE_DIR "/cats/009/Animation/attack4.png",
                RESOURCE_DIR "/cats/009/Animation/attack5.png",
                RESOURCE_DIR "/cats/009/Animation/attack6.png",
                RESOURCE_DIR "/cats/009/Animation/attack7.png",
                RESOURCE_DIR "/cats/009/Animation/attack8.png",
                RESOURCE_DIR "/cats/009/Animation/attack9.png",
                RESOURCE_DIR "/cats/009/Animation/attack9.png" // for padding
            });

        actress.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/009/Animation/hitback.png"});
    }

    {
        auto &kung_fu = s_anime[static_cast<size_t>(CatType::KUNG_FU_CAT)];

        kung_fu.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/010/Animation/walk0.png",
            });

        kung_fu.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/010/Animation/walk0.png",
                RESOURCE_DIR "/cats/010/Animation/walk1.png",
                RESOURCE_DIR "/cats/010/Animation/walk2.png",
                RESOURCE_DIR "/cats/010/Animation/walk3.png",
                RESOURCE_DIR "/cats/010/Animation/walk4.png",
                RESOURCE_DIR "/cats/010/Animation/walk5.png",
                RESOURCE_DIR "/cats/010/Animation/walk6.png",
            });

        kung_fu.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/010/Animation/attack0.png",
                RESOURCE_DIR "/cats/010/Animation/attack1.png",
                RESOURCE_DIR "/cats/010/Animation/attack2.png",
                RESOURCE_DIR "/cats/010/Animation/attack3.png",
                RESOURCE_DIR "/cats/010/Animation/attack4.png",
                RESOURCE_DIR "/cats/010/Animation/attack5.png",
                RESOURCE_DIR "/cats/010/Animation/attack6.png",
                RESOURCE_DIR "/cats/010/Animation/attack7.png",
                RESOURCE_DIR "/cats/010/Animation/attack8.png",
                RESOURCE_DIR "/cats/010/Animation/attack8.png" // for padding
            });

        kung_fu.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/010/Animation/hitback.png"});
    }

    {
        auto &mr = s_anime[static_cast<size_t>(CatType::MR_CAT)];

        mr.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/011/Animation/idle.png",
            });

        mr.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/011/Animation/walk0.png",
                RESOURCE_DIR "/cats/011/Animation/walk1.png",
                RESOURCE_DIR "/cats/011/Animation/walk2.png",
                RESOURCE_DIR "/cats/011/Animation/walk3.png",
                RESOURCE_DIR "/cats/011/Animation/walk4.png",
                RESOURCE_DIR "/cats/011/Animation/walk5.png",
                RESOURCE_DIR "/cats/011/Animation/walk6.png",
                RESOURCE_DIR "/cats/011/Animation/walk7.png",
                RESOURCE_DIR "/cats/011/Animation/walk8.png"
            });

        mr.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/011/Animation/attack0.png",
                RESOURCE_DIR "/cats/011/Animation/attack1.png",
                RESOURCE_DIR "/cats/011/Animation/attack2.png",
                RESOURCE_DIR "/cats/011/Animation/attack3.png",
                RESOURCE_DIR "/cats/011/Animation/attack4.png",
                RESOURCE_DIR "/cats/011/Animation/attack5.png",
                RESOURCE_DIR "/cats/011/Animation/attack6.png",
                RESOURCE_DIR "/cats/011/Animation/attack7.png",
                RESOURCE_DIR "/cats/011/Animation/attack7.png" // for padding
            });

        mr.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/011/Animation/hitback.png"});
    }

    {
        auto &bondage = s_anime[static_cast<size_t>(CatType::BONDAGE_CAT)];

        bondage.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/012/Animation/idle.png",
            });

        bondage.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/012/Animation/walk0.png",
                RESOURCE_DIR "/cats/012/Animation/walk1.png",
                RESOURCE_DIR "/cats/012/Animation/walk2.png",
                RESOURCE_DIR "/cats/012/Animation/walk3.png",
                RESOURCE_DIR "/cats/012/Animation/walk4.png",
                RESOURCE_DIR "/cats/012/Animation/walk5.png",
                RESOURCE_DIR "/cats/012/Animation/walk6.png",
                RESOURCE_DIR "/cats/012/Animation/walk7.png",
                RESOURCE_DIR "/cats/012/Animation/walk8.png"});

        bondage.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/012/Animation/attack0.png",
                RESOURCE_DIR "/cats/012/Animation/attack1.png",
                RESOURCE_DIR "/cats/012/Animation/attack2.png",
                RESOURCE_DIR "/cats/012/Animation/attack3.png",
                RESOURCE_DIR "/cats/012/Animation/attack4.png",
                RESOURCE_DIR "/cats/012/Animation/attack5.png",
                RESOURCE_DIR "/cats/012/Animation/attack6.png",
                RESOURCE_DIR "/cats/012/Animation/attack7.png",
                RESOURCE_DIR "/cats/012/Animation/attack8.png",
                RESOURCE_DIR "/cats/012/Animation/attack9.png",
                RESOURCE_DIR "/cats/012/Animation/attack10.png",
                RESOURCE_DIR "/cats/012/Animation/attack10.png" // for padding
            });

        bondage.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/012/Animation/hitback.png"});
    }

    {
        auto &dom = s_anime[static_cast<size_t>(CatType::DOM_CAT)];

        dom.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/013/Animation/idle.png",
            });

        dom.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/013/Animation/walk0.png",
                RESOURCE_DIR "/cats/013/Animation/walk1.png",
                RESOURCE_DIR "/cats/013/Animation/walk2.png",
                RESOURCE_DIR "/cats/013/Animation/walk3.png",
                RESOURCE_DIR "/cats/013/Animation/walk4.png",
                RESOURCE_DIR "/cats/013/Animation/walk5.png",
                RESOURCE_DIR "/cats/013/Animation/walk6.png",
                RESOURCE_DIR "/cats/013/Animation/walk7.png"});

        dom.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/013/Animation/attack0.png",
                RESOURCE_DIR "/cats/013/Animation/attack1.png",
                RESOURCE_DIR "/cats/013/Animation/attack2.png",
                RESOURCE_DIR "/cats/013/Animation/attack3.png",
                RESOURCE_DIR "/cats/013/Animation/attack4.png",
                RESOURCE_DIR "/cats/013/Animation/attack5.png",
                RESOURCE_DIR "/cats/013/Animation/attack6.png",
                RESOURCE_DIR "/cats/013/Animation/attack6.png" // for padding
            });

        dom.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/013/Animation/hitback.png"});
    }

    {
        auto &box = s_anime[static_cast<size_t>(CatType::CATS_IN_A_BOX)];

        box.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/014/Animation/idle.png",
            });

        box.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/014/Animation/walk0.png",
                RESOURCE_DIR "/cats/014/Animation/walk1.png",
                RESOURCE_DIR "/cats/014/Animation/walk2.png",
                RESOURCE_DIR "/cats/014/Animation/walk3.png",
                RESOURCE_DIR "/cats/014/Animation/walk4.png",
                RESOURCE_DIR "/cats/014/Animation/walk5.png",
                RESOURCE_DIR "/cats/014/Animation/walk6.png",
                RESOURCE_DIR "/cats/014/Animation/walk7.png"});

        box.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/014/Animation/attack0.png",
                RESOURCE_DIR "/cats/014/Animation/attack1.png",
                RESOURCE_DIR "/cats/014/Animation/attack2.png",
                RESOURCE_DIR "/cats/014/Animation/attack3.png",
                RESOURCE_DIR "/cats/014/Animation/attack4.png",
                RESOURCE_DIR "/cats/014/Animation/attack5.png",
                RESOURCE_DIR "/cats/014/Animation/attack6.png",
                RESOURCE_DIR "/cats/014/Animation/attack7.png",
                RESOURCE_DIR "/cats/014/Animation/attack8.png",
                RESOURCE_DIR "/cats/014/Animation/attack9.png",
                RESOURCE_DIR "/cats/014/Animation/attack10.png",
                RESOURCE_DIR "/cats/014/Animation/attack11.png",
                RESOURCE_DIR "/cats/014/Animation/attack12.png",
                RESOURCE_DIR "/cats/014/Animation/attack13.png",
                RESOURCE_DIR "/cats/014/Animation/attack13.png" // for padding
            });

        box.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/014/Animation/hitback.png"});
    }

    {
        auto &skirt = s_anime[static_cast<size_t>(CatType::PANTIES_CAT)];

        skirt.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/015/Animation/idle.png",
            });

        skirt.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/015/Animation/walk0.png",
                RESOURCE_DIR "/cats/015/Animation/walk1.png",
                RESOURCE_DIR "/cats/015/Animation/walk2.png",
                RESOURCE_DIR "/cats/015/Animation/walk3.png",
                RESOURCE_DIR "/cats/015/Animation/walk4.png",
                RESOURCE_DIR "/cats/015/Animation/walk5.png",
                RESOURCE_DIR "/cats/015/Animation/walk6.png"});

        skirt.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/015/Animation/attack0.png",
                RESOURCE_DIR "/cats/015/Animation/attack1.png",
                RESOURCE_DIR "/cats/015/Animation/attack2.png",
                RESOURCE_DIR "/cats/015/Animation/attack3.png",
                RESOURCE_DIR "/cats/015/Animation/attack4.png",
                RESOURCE_DIR "/cats/015/Animation/attack4.png" // for padding
            });

        skirt.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/015/Animation/hitback.png"});
    }

    {
        auto &moneko = s_anime[static_cast<size_t>(CatType::MONEKO)];

        moneko.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/016/Animation/idle.png",
            });

        moneko.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/016/Animation/walk0.png",
                RESOURCE_DIR "/cats/016/Animation/walk1.png",
                RESOURCE_DIR "/cats/016/Animation/walk2.png",
                RESOURCE_DIR "/cats/016/Animation/walk3.png",
                RESOURCE_DIR "/cats/016/Animation/walk4.png",
                RESOURCE_DIR "/cats/016/Animation/walk5.png",
                RESOURCE_DIR "/cats/016/Animation/walk6.png",
                RESOURCE_DIR "/cats/016/Animation/walk7.png",
                RESOURCE_DIR "/cats/016/Animation/walk8.png",
                RESOURCE_DIR "/cats/016/Animation/walk9.png"});

        moneko.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/016/Animation/attack0.png",
                RESOURCE_DIR "/cats/016/Animation/attack1.png",
                RESOURCE_DIR "/cats/016/Animation/attack2.png",
                RESOURCE_DIR "/cats/016/Animation/attack3.png",
                RESOURCE_DIR "/cats/016/Animation/attack4.png",
                RESOURCE_DIR "/cats/016/Animation/attack5.png",
                RESOURCE_DIR "/cats/016/Animation/attack6.png",
                RESOURCE_DIR "/cats/016/Animation/attack7.png",
                RESOURCE_DIR "/cats/016/Animation/attack8.png",
                RESOURCE_DIR "/cats/016/Animation/attack9.png",
                RESOURCE_DIR "/cats/016/Animation/attack10.png",
                RESOURCE_DIR "/cats/016/Animation/attack11.png",
                RESOURCE_DIR "/cats/016/Animation/attack12.png",
                RESOURCE_DIR "/cats/016/Animation/attack13.png",
                RESOURCE_DIR "/cats/016/Animation/attack14.png",
                RESOURCE_DIR "/cats/016/Animation/attack15.png",
                RESOURCE_DIR "/cats/016/Animation/attack16.png",
                RESOURCE_DIR "/cats/016/Animation/attack16.png" // for padding
            });

        moneko.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/016/Animation/hitback.png"});
    }
}

const Cat::Animation CatAnimeResource::Get(const CatType type) {
    if (!s_init) {
        Init();
        s_init = true;
    }
    const auto &anime = s_anime[static_cast<size_t>(type)];

    Cat::Animation obj;
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
