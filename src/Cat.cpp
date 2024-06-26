#include "Cat.hpp"
#include "DebugUtil/BattleLog.hpp"
#include "Sound.hpp"
//#include <random>

Cat::Cat(const CatType type, const int level)
    : m_Type(type) {
    SetStats(BaseCatStats::Stats[static_cast<size_t>(type)]);
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

    case CatType::TRICYCLE_CAT:
        m_Anime = CatAnime::Tricycle();
        break;

    case CatType::NINJA_CAT:
        m_Anime = CatAnime::Ninja();
        break;

    case CatType::ZOMBIE_CAT:
        m_Anime = CatAnime::Zombie();
        break;

    case CatType::SAMURAI_CAT:
        m_Anime = CatAnime::Samurai();
        break;

    case CatType::SUMO_CAT:
        m_Anime = CatAnime::Sumo();
        break;

    case CatType::BOOGIE_CAT:
        m_Anime = CatAnime::Boogie();
        break;

    case CatType::SKIRT_CAT:
        m_Anime = CatAnime::Skirt();
        break;

    case CatType::MACHO_CAT:
        m_Anime = CatAnime::Macho();
        break;

    case CatType::WALL_CAT:
        m_Anime = CatAnime::Wall();
        break;

    case CatType::BRAVE_CAT:
        m_Anime = CatAnime::Brave();
        break;

    case CatType::SEXY_LEGS_CAT:
        m_Anime = CatAnime::SexyLegs();
        break;

    case CatType::GIRAFFE_CAT:
        m_Anime = CatAnime::GiraffeCat();
        break;

    case CatType::UFO_CAT:
        m_Anime = CatAnime::UfoCat();
        break;

    case CatType::WHALE_CAT:
        m_Anime = CatAnime::WhaleCat();
        break;

    case CatType::DRAGON_CAT:
        m_Anime = CatAnime::DragonCat();
        break;

    case CatType::MYTHICAL_TITAN_CAT:
        m_Anime = CatAnime::MythicalTitanCat();
        break;

    case CatType::MOTHER_CAT:
        m_Anime = CatAnime::MotherCat();
        break;

    case CatType::DRUNKEN_MASTER_CAT:
        m_Anime = CatAnime::DrunkenMasterCat();
        break;

    case CatType::SUPER_MR_CAT:
        m_Anime = CatAnime::SuperMrCat();
        break;

    case CatType::BONDAGE_CATNEO:
        m_Anime = CatAnime::BondageCatneo();
        break;

    case CatType::EXECUTIONER:
        m_Anime = CatAnime::Executioner();
        break;

    case CatType::CAT_GANG:
        m_Anime = CatAnime::CatGang();
        break;

    case CatType::LEAF_CAT:
        m_Anime = CatAnime::LeafCat();
        break;

    case CatType::MISS_MONEKO:
        m_Anime = CatAnime::MissMoneko();
        break;

    case CatType::BIKER_CAT:
        m_Anime = CatAnime::BikerCat();
        break;

    case CatType::NINJA_FROG_CAT:
        m_Anime = CatAnime::NinjaFrogCat();
        break;

    case CatType::DEVIL_CAT:
        m_Anime = CatAnime::DevilCat();
        break;

    case CatType::KNIGHT_CAT:
        m_Anime = CatAnime::KnightCat();
        break;

    case CatType::MADAME_SUMO_CAT:
        m_Anime = CatAnime::MadameSumoCat();
        break;

    case CatType::SAMBA_CAT:
        m_Anime = CatAnime::SambaCat();
        break;

    case CatType::TIGHTS_CAT:
        m_Anime = CatAnime::TightsCat();
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

    {
        auto &cycle = s_anime[static_cast<size_t>(CatType::TRICYCLE_CAT)];

        cycle.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/017/Animation/idle.png",
            });

        cycle.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/017/Animation/walk0.png",
                RESOURCE_DIR "/cats/017/Animation/walk1.png",
                RESOURCE_DIR "/cats/017/Animation/walk2.png",
                RESOURCE_DIR "/cats/017/Animation/walk3.png",
                RESOURCE_DIR "/cats/017/Animation/walk4.png",
                RESOURCE_DIR "/cats/017/Animation/walk5.png",
                RESOURCE_DIR "/cats/017/Animation/walk6.png"});

        cycle.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/017/Animation/attack0.png",
                RESOURCE_DIR "/cats/017/Animation/attack1.png",
                RESOURCE_DIR "/cats/017/Animation/attack2.png",
                RESOURCE_DIR "/cats/017/Animation/attack3.png",
                RESOURCE_DIR "/cats/017/Animation/attack4.png",
                RESOURCE_DIR "/cats/017/Animation/attack5.png",
                RESOURCE_DIR "/cats/017/Animation/attack6.png",
                RESOURCE_DIR "/cats/017/Animation/attack7.png",
                RESOURCE_DIR "/cats/017/Animation/attack8.png",
                RESOURCE_DIR "/cats/017/Animation/attack9.png",
                RESOURCE_DIR "/cats/017/Animation/attack10.png",
                RESOURCE_DIR "/cats/017/Animation/attack11.png",
                RESOURCE_DIR "/cats/017/Animation/attack11.png" // for padding
            });

        cycle.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/017/Animation/hitback.png"});
    }

    {
        auto &ninja = s_anime[static_cast<size_t>(CatType::NINJA_CAT)];

        ninja.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/018/Animation/idle.png",
            });

        ninja.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/018/Animation/walk0.png",
                RESOURCE_DIR "/cats/018/Animation/walk1.png",
                RESOURCE_DIR "/cats/018/Animation/walk2.png",
                RESOURCE_DIR "/cats/018/Animation/walk3.png",
                RESOURCE_DIR "/cats/018/Animation/walk4.png"});

        ninja.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/018/Animation/attack0.png",
                RESOURCE_DIR "/cats/018/Animation/attack1.png",
                RESOURCE_DIR "/cats/018/Animation/attack2.png",
                RESOURCE_DIR "/cats/018/Animation/attack3.png",
                RESOURCE_DIR "/cats/018/Animation/attack4.png",
                RESOURCE_DIR "/cats/018/Animation/attack5.png",
                RESOURCE_DIR "/cats/018/Animation/attack6.png",
                RESOURCE_DIR "/cats/018/Animation/attack7.png",
                RESOURCE_DIR "/cats/018/Animation/attack7.png" // for padding
            });

        ninja.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/018/Animation/hitback.png"});
    }

    {
        auto &zombie = s_anime[static_cast<size_t>(CatType::ZOMBIE_CAT)];

        zombie.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/019/Animation/idle.png",
            });

        zombie.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/019/Animation/walk0.png",
                RESOURCE_DIR "/cats/019/Animation/walk1.png",
                RESOURCE_DIR "/cats/019/Animation/walk2.png",
                RESOURCE_DIR "/cats/019/Animation/walk3.png",
                RESOURCE_DIR "/cats/019/Animation/walk4.png",
                RESOURCE_DIR "/cats/019/Animation/walk5.png",
                RESOURCE_DIR "/cats/019/Animation/walk6.png",
                RESOURCE_DIR "/cats/019/Animation/walk7.png",
                RESOURCE_DIR "/cats/019/Animation/walk8.png",
                RESOURCE_DIR "/cats/019/Animation/walk9.png",
                RESOURCE_DIR "/cats/019/Animation/walk10.png"});

        zombie.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/019/Animation/attack0.png",
                RESOURCE_DIR "/cats/019/Animation/attack1.png",
                RESOURCE_DIR "/cats/019/Animation/attack2.png",
                RESOURCE_DIR "/cats/019/Animation/attack3.png",
                RESOURCE_DIR "/cats/019/Animation/attack4.png",
                RESOURCE_DIR "/cats/019/Animation/attack5.png",
                RESOURCE_DIR "/cats/019/Animation/attack6.png",
                RESOURCE_DIR "/cats/019/Animation/attack7.png",
                RESOURCE_DIR "/cats/019/Animation/attack8.png",
                RESOURCE_DIR "/cats/019/Animation/attack8.png" // for padding
            });

        zombie.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/019/Animation/hitback.png"});
    }

    {
        auto &samurai = s_anime[static_cast<size_t>(CatType::SAMURAI_CAT)];

        samurai.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/020/Animation/idle.png",
            });

        samurai.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/020/Animation/walk0.png",
                RESOURCE_DIR "/cats/020/Animation/walk1.png",
                RESOURCE_DIR "/cats/020/Animation/walk2.png",
                RESOURCE_DIR "/cats/020/Animation/walk3.png",
                RESOURCE_DIR "/cats/020/Animation/walk4.png",
                RESOURCE_DIR "/cats/020/Animation/walk5.png",
                RESOURCE_DIR "/cats/020/Animation/walk6.png",
                RESOURCE_DIR "/cats/020/Animation/walk7.png"});

        samurai.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/020/Animation/attack0.png",
                RESOURCE_DIR "/cats/020/Animation/attack1.png",
                RESOURCE_DIR "/cats/020/Animation/attack2.png",
                RESOURCE_DIR "/cats/020/Animation/attack3.png",
                RESOURCE_DIR "/cats/020/Animation/attack4.png",
                RESOURCE_DIR "/cats/020/Animation/attack5.png",
                RESOURCE_DIR "/cats/020/Animation/attack6.png",
                RESOURCE_DIR "/cats/020/Animation/attack7.png",
                RESOURCE_DIR "/cats/020/Animation/attack8.png",
                RESOURCE_DIR "/cats/020/Animation/attack8.png" // for padding
            });

        samurai.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/020/Animation/hitback.png"});
    }

    {
        auto &sumo = s_anime[static_cast<size_t>(CatType::SUMO_CAT)];

        sumo.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/021/Animation/idle.png",
            });

        sumo.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/021/Animation/walk0.png",
                RESOURCE_DIR "/cats/021/Animation/walk1.png",
                RESOURCE_DIR "/cats/021/Animation/walk2.png",
                RESOURCE_DIR "/cats/021/Animation/walk3.png",
                RESOURCE_DIR "/cats/021/Animation/walk4.png",
                RESOURCE_DIR "/cats/021/Animation/walk5.png"});

        sumo.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/021/Animation/attack0.png",
                RESOURCE_DIR "/cats/021/Animation/attack1.png",
                RESOURCE_DIR "/cats/021/Animation/attack2.png",
                RESOURCE_DIR "/cats/021/Animation/attack3.png",
                RESOURCE_DIR "/cats/021/Animation/attack4.png",
                RESOURCE_DIR "/cats/021/Animation/attack5.png",
                RESOURCE_DIR "/cats/021/Animation/attack6.png",
                RESOURCE_DIR "/cats/021/Animation/attack7.png",
                RESOURCE_DIR "/cats/021/Animation/attack8.png",
                RESOURCE_DIR "/cats/021/Animation/attack9.png",
                RESOURCE_DIR "/cats/021/Animation/attack9.png" // for padding
            });

        sumo.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/021/Animation/hitback.png"});
    }

    {
        auto &boogie = s_anime[static_cast<size_t>(CatType::BOOGIE_CAT)];

        boogie.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/022/Animation/idle.png",
            });

        boogie.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/022/Animation/walk0.png",
                RESOURCE_DIR "/cats/022/Animation/walk1.png",
                RESOURCE_DIR "/cats/022/Animation/walk2.png",
                RESOURCE_DIR "/cats/022/Animation/walk3.png"});

        boogie.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/022/Animation/attack0.png",
                RESOURCE_DIR "/cats/022/Animation/attack1.png",
                RESOURCE_DIR "/cats/022/Animation/attack2.png",
                RESOURCE_DIR "/cats/022/Animation/attack3.png",
                RESOURCE_DIR "/cats/022/Animation/attack4.png",
                RESOURCE_DIR "/cats/022/Animation/attack5.png",
                RESOURCE_DIR "/cats/022/Animation/attack6.png",
                RESOURCE_DIR "/cats/022/Animation/attack7.png",
                RESOURCE_DIR "/cats/022/Animation/attack7.png" // for padding
            });

        boogie.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/022/Animation/hitback.png"});
    }

    {
        auto &skirt = s_anime[static_cast<size_t>(CatType::SKIRT_CAT)];

        skirt.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/023/Animation/idle.png",
            });

        skirt.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/023/Animation/walk0.png",
                RESOURCE_DIR "/cats/023/Animation/walk1.png",
                RESOURCE_DIR "/cats/023/Animation/walk2.png",
                RESOURCE_DIR "/cats/023/Animation/walk3.png",
                RESOURCE_DIR "/cats/023/Animation/walk4.png",
                RESOURCE_DIR "/cats/023/Animation/walk5.png"
            });

        skirt.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/023/Animation/attack0.png",
                RESOURCE_DIR "/cats/023/Animation/attack1.png",
                RESOURCE_DIR "/cats/023/Animation/attack2.png",
                RESOURCE_DIR "/cats/023/Animation/attack3.png",
                RESOURCE_DIR "/cats/023/Animation/attack4.png",
                RESOURCE_DIR "/cats/023/Animation/attack4.png" // for padding
            });

        skirt.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/023/Animation/hitback.png"});
    }

    {
        auto &macho = s_anime[static_cast<size_t>(CatType::MACHO_CAT)];

        macho.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/024/Animation/idle.png",
            });

        macho.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/024/Animation/walk0.png",
                RESOURCE_DIR "/cats/024/Animation/walk1.png",
                RESOURCE_DIR "/cats/024/Animation/walk2.png",
                RESOURCE_DIR "/cats/024/Animation/walk3.png"});

        macho.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/024/Animation/attack0.png",
                RESOURCE_DIR "/cats/024/Animation/attack1.png",
                RESOURCE_DIR "/cats/024/Animation/attack2.png",
                RESOURCE_DIR "/cats/024/Animation/attack3.png",
                RESOURCE_DIR "/cats/024/Animation/attack4.png",
                RESOURCE_DIR "/cats/024/Animation/attack4.png" // for padding
            });

        macho.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/024/Animation/hitback.png"});
    }

    {
        auto &wall = s_anime[static_cast<size_t>(CatType::WALL_CAT)];

        wall.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/025/Animation/idle.png",
            });

        wall.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/025/Animation/walk1.png",
                RESOURCE_DIR "/cats/025/Animation/walk2.png",
                RESOURCE_DIR "/cats/025/Animation/walk3.png",
                RESOURCE_DIR "/cats/025/Animation/walk4.png"});

        wall.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/025/Animation/attack0.png",
                RESOURCE_DIR "/cats/025/Animation/attack1.png",
                RESOURCE_DIR "/cats/025/Animation/attack2.png",
                RESOURCE_DIR "/cats/025/Animation/attack3.png",
                RESOURCE_DIR "/cats/025/Animation/attack4.png",
                RESOURCE_DIR "/cats/025/Animation/attack5.png",
                RESOURCE_DIR "/cats/025/Animation/attack6.png",
                RESOURCE_DIR "/cats/025/Animation/attack6.png" // for padding
            });

        wall.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/025/Animation/hitback.png"});
    }

    {
        auto &brave = s_anime[static_cast<size_t>(CatType::BRAVE_CAT)];

        brave.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/026/Animation/idle.png",
            });

        brave.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/026/Animation/walk0.png",
                RESOURCE_DIR "/cats/026/Animation/walk1.png",
                RESOURCE_DIR "/cats/026/Animation/walk2.png",
                RESOURCE_DIR "/cats/026/Animation/walk3.png"});

        brave.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/026/Animation/attack0.png",
                RESOURCE_DIR "/cats/026/Animation/attack1.png",
                RESOURCE_DIR "/cats/026/Animation/attack2.png",
                RESOURCE_DIR "/cats/026/Animation/attack3.png",
                RESOURCE_DIR "/cats/026/Animation/attack4.png",
                RESOURCE_DIR "/cats/026/Animation/attack4.png" // for padding
            });

        brave.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/026/Animation/hitback.png"});
    }

    {
        auto &sexy = s_anime[static_cast<size_t>(CatType::SEXY_LEGS_CAT)];

        sexy.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/027/Animation/idle.png",
            });

        sexy.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/027/Animation/walk0.png",
                RESOURCE_DIR "/cats/027/Animation/walk1.png",
                RESOURCE_DIR "/cats/027/Animation/walk2.png",
                RESOURCE_DIR "/cats/027/Animation/walk3.png",
                RESOURCE_DIR "/cats/027/Animation/walk4.png",
                RESOURCE_DIR "/cats/027/Animation/walk5.png"});

        sexy.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/027/Animation/attack0.png",
                RESOURCE_DIR "/cats/027/Animation/attack1.png",
                RESOURCE_DIR "/cats/027/Animation/attack2.png",
                RESOURCE_DIR "/cats/027/Animation/attack3.png",
                RESOURCE_DIR "/cats/027/Animation/attack4.png",
                RESOURCE_DIR "/cats/027/Animation/attack4.png" // for padding
            });

        sexy.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/027/Animation/hitback.png"});
    }

    {
        auto &giraffe = s_anime[static_cast<size_t>(CatType::GIRAFFE_CAT)];

        giraffe.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/028/idle.png"
            });

        giraffe.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/028/walk0.png",
                RESOURCE_DIR "/cats/028/walk1.png",
                RESOURCE_DIR "/cats/028/walk2.png",
                RESOURCE_DIR "/cats/028/walk3.png",
                RESOURCE_DIR "/cats/028/walk4.png"});

        giraffe.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/028/attack0.png",
                RESOURCE_DIR "/cats/028/attack1.png",
                RESOURCE_DIR "/cats/028/attack2.png",
                RESOURCE_DIR "/cats/028/attack3.png",
                RESOURCE_DIR "/cats/028/attack3.png" // for padding
            });

        giraffe.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/028/hitback.png"});
    }
    //-------------------------------------------------------------------
    {
        auto &ufocat = s_anime[static_cast<size_t>(CatType::UFO_CAT)];

        ufocat.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/029/Animation/idle0.png",
                RESOURCE_DIR"/cats/029/Animation/idle1.png",
                RESOURCE_DIR"/cats/029/Animation/idle2.png",
                RESOURCE_DIR"/cats/029/Animation/idle3.png"
            });

        ufocat.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/029/Animation/walk0.png",
                RESOURCE_DIR"/cats/029/Animation/walk1.png",
                RESOURCE_DIR"/cats/029/Animation/walk2.png",
                RESOURCE_DIR"/cats/029/Animation/walk3.png"});

        ufocat.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/029/Animation/attack0.png",
                RESOURCE_DIR"/cats/029/Animation/attack1.png",
                RESOURCE_DIR"/cats/029/Animation/attack2.png",
                RESOURCE_DIR"/cats/029/Animation/attack3.png",
                RESOURCE_DIR"/cats/029/Animation/attack5.png",
                RESOURCE_DIR"/cats/029/Animation/attack6.png",
                RESOURCE_DIR"/cats/029/Animation/attack7.png",
                RESOURCE_DIR"/cats/029/Animation/attack8.png",
                RESOURCE_DIR"/cats/029/Animation/attack9.png",
                RESOURCE_DIR"/cats/029/Animation/attack10.png",
                RESOURCE_DIR"/cats/029/Animation/attack10.png"
                // for padding
            });

        ufocat.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/029/Animation/hitback.png"});
    }
    {
        auto &whalecat = s_anime[static_cast<size_t>(CatType::WHALE_CAT)];

        whalecat.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/030/Animation/idle.png"
            });

        whalecat.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/030/Animation/walk0.png",
                RESOURCE_DIR"/cats/030/Animation/walk1.png",
                RESOURCE_DIR"/cats/030/Animation/walk2.png",
                RESOURCE_DIR"/cats/030/Animation/walk3.png",
                RESOURCE_DIR"/cats/030/Animation/walk4.png",
                RESOURCE_DIR"/cats/030/Animation/walk5.png",
                RESOURCE_DIR"/cats/030/Animation/walk6.png",
                RESOURCE_DIR"/cats/030/Animation/walk7.png"
            });

        whalecat.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/030/Animation/attack0.png",
                RESOURCE_DIR"/cats/030/Animation/attack1.png",
                RESOURCE_DIR"/cats/030/Animation/attack2.png",
                RESOURCE_DIR"/cats/030/Animation/attack3.png",
                RESOURCE_DIR"/cats/030/Animation/attack4.png",
                RESOURCE_DIR"/cats/030/Animation/attack5.png",
                RESOURCE_DIR"/cats/030/Animation/attack5.png"// for padding
            });

        whalecat.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/030/Animation/hitback.png"});
    }
    {
        auto &dragoncat = s_anime[static_cast<size_t>(CatType::DRAGON_CAT)];

        dragoncat.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/031/Animation/idle.png"
            });

        dragoncat.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/031/Animation/walk0.png",
                RESOURCE_DIR"/cats/031/Animation/walk1.png",
                RESOURCE_DIR"/cats/031/Animation/walk2.png",
                RESOURCE_DIR"/cats/031/Animation/walk3.png",
                RESOURCE_DIR"/cats/031/Animation/walk4.png"});

        dragoncat.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/031/Animation/attack0.png",
                RESOURCE_DIR"/cats/031/Animation/attack1.png",
                RESOURCE_DIR"/cats/031/Animation/attack2.png",
                RESOURCE_DIR"/cats/031/Animation/attack3.png",
                RESOURCE_DIR"/cats/031/Animation/attack4.png",
                RESOURCE_DIR"/cats/031/Animation/attack5.png",
                RESOURCE_DIR"/cats/031/Animation/attack5.png"// for padding
            });

        dragoncat.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/031/Animation/hitback.png"});
    }
    {
        auto &mythical_titan_cat = s_anime[static_cast<size_t>(CatType::MYTHICAL_TITAN_CAT)];

        mythical_titan_cat.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/032/Animation/idle.png"
            });

        mythical_titan_cat.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/032/Animation/walk0.png",
                RESOURCE_DIR"/cats/032/Animation/walk1.png",
                RESOURCE_DIR"/cats/032/Animation/walk2.png",
                RESOURCE_DIR"/cats/032/Animation/walk3.png",
                RESOURCE_DIR"/cats/032/Animation/walk4.png",
                RESOURCE_DIR"/cats/032/Animation/walk5.png",
                RESOURCE_DIR"/cats/032/Animation/walk6.png"
                });

        mythical_titan_cat.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/032/Animation/attack0.png",
                RESOURCE_DIR"/cats/032/Animation/attack1.png",
                RESOURCE_DIR"/cats/032/Animation/attack2.png",
                RESOURCE_DIR"/cats/032/Animation/attack3.png",
                RESOURCE_DIR"/cats/032/Animation/attack4.png",
                RESOURCE_DIR"/cats/032/Animation/attack5.png",
                RESOURCE_DIR"/cats/032/Animation/attack5.png"// for padding
            });

        mythical_titan_cat.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/032/Animation/hitback.png"});
    }
    {
        auto &mother_cat = s_anime[static_cast<size_t>(CatType::MOTHER_CAT)];

        mother_cat.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/033/Animation/idle.png"
            });

        mother_cat.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/033/Animation/walk0.png",
                RESOURCE_DIR"/cats/033/Animation/walk1.png",
                RESOURCE_DIR"/cats/033/Animation/walk2.png",
                RESOURCE_DIR"/cats/033/Animation/walk3.png",
                RESOURCE_DIR"/cats/033/Animation/walk4.png",
                RESOURCE_DIR"/cats/033/Animation/walk5.png",
                RESOURCE_DIR"/cats/033/Animation/walk6.png",
                RESOURCE_DIR"/cats/033/Animation/walk7.png",
                RESOURCE_DIR"/cats/033/Animation/walk8.png",
                RESOURCE_DIR"/cats/033/Animation/walk9.png"});

        mother_cat.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/033/Animation/attack0.png",
                RESOURCE_DIR"/cats/033/Animation/attack1.png",
                RESOURCE_DIR"/cats/033/Animation/attack2.png",
                RESOURCE_DIR"/cats/033/Animation/attack3.png",
                RESOURCE_DIR"/cats/033/Animation/attack4.png",
                RESOURCE_DIR"/cats/033/Animation/attack5.png",
                RESOURCE_DIR"/cats/033/Animation/attack6.png",
                RESOURCE_DIR"/cats/033/Animation/attack7.png",
                RESOURCE_DIR"/cats/033/Animation/attack8.png",
                RESOURCE_DIR"/cats/033/Animation/attack9.png",
                RESOURCE_DIR"/cats/033/Animation/attack10.png",
                RESOURCE_DIR"/cats/033/Animation/attack11.png",
                RESOURCE_DIR"/cats/033/Animation/attack12.png",
                RESOURCE_DIR"/cats/033/Animation/attack13.png",
                RESOURCE_DIR"/cats/033/Animation/attack14.png",
                RESOURCE_DIR"/cats/033/Animation/attack14.png"
                // for padding
            });

        mother_cat.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/033/Animation/hitback.png"});
    }
    {
        auto &drunken_master_cat = s_anime[static_cast<size_t>(CatType::DRUNKEN_MASTER_CAT)];

        drunken_master_cat.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/034/Animation/idle0.png",
                RESOURCE_DIR"/cats/034/Animation/idle1.png",
                RESOURCE_DIR"/cats/034/Animation/idle2.png",
                RESOURCE_DIR"/cats/034/Animation/idle3.png",
                RESOURCE_DIR"/cats/034/Animation/idle4.png",
                RESOURCE_DIR"/cats/034/Animation/idle5.png"
            });

        drunken_master_cat.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/034/Animation/walk0.png",
                RESOURCE_DIR"/cats/034/Animation/walk1.png",
                RESOURCE_DIR"/cats/034/Animation/walk2.png",
                RESOURCE_DIR"/cats/034/Animation/walk3.png",
                RESOURCE_DIR"/cats/034/Animation/walk4.png",
                RESOURCE_DIR"/cats/034/Animation/walk5.png",
                RESOURCE_DIR"/cats/034/Animation/walk6.png",
                RESOURCE_DIR"/cats/034/Animation/walk7.png",
                RESOURCE_DIR"/cats/034/Animation/walk8.png",
                RESOURCE_DIR"/cats/034/Animation/walk9.png",
                RESOURCE_DIR"/cats/034/Animation/walk10.png",
                RESOURCE_DIR"/cats/034/Animation/walk11.png",
                RESOURCE_DIR"/cats/034/Animation/walk12.png"
            });

        drunken_master_cat.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/034/Animation/attack0.png",
                RESOURCE_DIR"/cats/034/Animation/attack1.png",
                RESOURCE_DIR"/cats/034/Animation/attack2.png",
                RESOURCE_DIR"/cats/034/Animation/attack3.png",
                RESOURCE_DIR"/cats/034/Animation/attack4.png",
                RESOURCE_DIR"/cats/034/Animation/attack5.png",
                RESOURCE_DIR"/cats/034/Animation/attack6.png",
                RESOURCE_DIR"/cats/034/Animation/attack7.png",
                RESOURCE_DIR"/cats/034/Animation/attack8.png",
                RESOURCE_DIR"/cats/034/Animation/attack9.png",
                RESOURCE_DIR"/cats/034/Animation/attack10.png",
                RESOURCE_DIR"/cats/034/Animation/attack11.png",
                RESOURCE_DIR"/cats/034/Animation/attack12.png",
                RESOURCE_DIR"/cats/034/Animation/attack13.png",
                RESOURCE_DIR"/cats/034/Animation/attack14.png",
                RESOURCE_DIR"/cats/034/Animation/attack15.png",
                RESOURCE_DIR"/cats/034/Animation/attack16.png",
                RESOURCE_DIR"/cats/034/Animation/attack17.png",
                RESOURCE_DIR"/cats/034/Animation/attack17.png"// for padding
            });

        drunken_master_cat.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/034/Animation/hitback.png"});
    }
    {
        auto &super_mr_cat = s_anime[static_cast<size_t>(CatType::SUPER_MR_CAT)];

        super_mr_cat.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/035/Animation/idle.png"
            });

        super_mr_cat.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/035/Animation/walk0.png",
                RESOURCE_DIR"/cats/035/Animation/walk1.png",
                RESOURCE_DIR"/cats/035/Animation/walk2.png",
                RESOURCE_DIR"/cats/035/Animation/walk3.png"});

        super_mr_cat.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/035/Animation/attack0.png",
                RESOURCE_DIR"/cats/035/Animation/attack1.png",
                RESOURCE_DIR"/cats/035/Animation/attack2.png",
                RESOURCE_DIR"/cats/035/Animation/attack3.png",
                RESOURCE_DIR"/cats/035/Animation/attack4.png",
                RESOURCE_DIR"/cats/035/Animation/attack5.png",
                RESOURCE_DIR"/cats/035/Animation/attack6.png",
                RESOURCE_DIR"/cats/035/Animation/attack7.png",
                RESOURCE_DIR"/cats/035/Animation/attack8.png",
                RESOURCE_DIR"/cats/035/Animation/attack8.png"// for padding
            });

        super_mr_cat.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/035/Animation/hitback.png",});
    }
    {
        auto &boddage_catneo = s_anime[static_cast<size_t>(CatType::BONDAGE_CATNEO)];

        boddage_catneo.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/036/Animation/idle.png",
            });

        boddage_catneo.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/036/Animation/walk0.png",
                RESOURCE_DIR"/cats/036/Animation/walk1.png",
                RESOURCE_DIR"/cats/036/Animation/walk2.png",
                RESOURCE_DIR"/cats/036/Animation/walk3.png",
                RESOURCE_DIR"/cats/036/Animation/walk4.png",
                RESOURCE_DIR"/cats/036/Animation/walk5.png",
                RESOURCE_DIR"/cats/036/Animation/walk6.png",
                RESOURCE_DIR"/cats/036/Animation/walk7.png"});

        boddage_catneo.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/036/Animation/attack0.png",
                RESOURCE_DIR"/cats/036/Animation/attack1.png",
                RESOURCE_DIR"/cats/036/Animation/attack2.png",
                RESOURCE_DIR"/cats/036/Animation/attack3.png",
                RESOURCE_DIR"/cats/036/Animation/attack4.png",
                RESOURCE_DIR"/cats/036/Animation/attack4.png"// for padding
            });

        boddage_catneo.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/036/Animation/hitback.png",});
    }
    {
        auto &executioner = s_anime[static_cast<size_t>(CatType::EXECUTIONER)];

        executioner.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/037/Animation/idle.png"
            });

        executioner.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/037/Animation/walk0.png",
                RESOURCE_DIR"/cats/037/Animation/walk1.png",
                RESOURCE_DIR"/cats/037/Animation/walk2.png",
                RESOURCE_DIR"/cats/037/Animation/walk3.png",
                RESOURCE_DIR"/cats/037/Animation/walk4.png",
                RESOURCE_DIR"/cats/037/Animation/walk5.png",
                RESOURCE_DIR"/cats/037/Animation/walk6.png",
                RESOURCE_DIR"/cats/037/Animation/walk7.png",
                RESOURCE_DIR"/cats/037/Animation/walk8.png",
                RESOURCE_DIR"/cats/037/Animation/walk9.png",
                RESOURCE_DIR"/cats/037/Animation/walk10.png",
                RESOURCE_DIR"/cats/037/Animation/walk11.png"});

        executioner.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/037/Animation/attack0.png",
                RESOURCE_DIR"/cats/037/Animation/attack1.png",
                RESOURCE_DIR"/cats/037/Animation/attack2.png",
                RESOURCE_DIR"/cats/037/Animation/attack3.png",
                RESOURCE_DIR"/cats/037/Animation/attack4.png",
                RESOURCE_DIR"/cats/037/Animation/attack5.png",
                RESOURCE_DIR"/cats/037/Animation/attack6.png",
                RESOURCE_DIR"/cats/037/Animation/attack7.png",
                RESOURCE_DIR"/cats/037/Animation/attack8.png",
                RESOURCE_DIR"/cats/037/Animation/attack8.png"// for padding
            });

        executioner.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/037/Animation/hitback.png"});
    }
    {
        auto &cat_gang = s_anime[static_cast<size_t>(CatType::CAT_GANG)];

        cat_gang.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/038/Animation/idle.png"
            });

        cat_gang.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/038/Animation/walk0.png",
                RESOURCE_DIR"/cats/038/Animation/walk1.png",
                RESOURCE_DIR"/cats/038/Animation/walk2.png",
                RESOURCE_DIR"/cats/038/Animation/walk3.png",
                RESOURCE_DIR"/cats/038/Animation/walk4.png",
                RESOURCE_DIR"/cats/038/Animation/walk5.png",
                RESOURCE_DIR"/cats/038/Animation/walk6.png",
                RESOURCE_DIR"/cats/038/Animation/walk7.png",
                RESOURCE_DIR"/cats/038/Animation/walk8.png"});

        cat_gang.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/038/Animation/attack0.png",
                RESOURCE_DIR"/cats/038/Animation/attack1.png",
                RESOURCE_DIR"/cats/038/Animation/attack2.png",
                RESOURCE_DIR"/cats/038/Animation/attack3.png",
                RESOURCE_DIR"/cats/038/Animation/attack4.png",
                RESOURCE_DIR"/cats/038/Animation/attack5.png",
                RESOURCE_DIR"/cats/038/Animation/attack6.png",
                RESOURCE_DIR"/cats/038/Animation/attack7.png",
                RESOURCE_DIR"/cats/038/Animation/attack8.png",
                RESOURCE_DIR"/cats/038/Animation/attack9.png",
                RESOURCE_DIR"/cats/038/Animation/attack10.png",
                RESOURCE_DIR"/cats/038/Animation/attack11.png",
                RESOURCE_DIR"/cats/038/Animation/attack12.png",
                RESOURCE_DIR"/cats/038/Animation/attack13.png",
                RESOURCE_DIR"/cats/038/Animation/attack14.png",
                RESOURCE_DIR"/cats/038/Animation/attack15.png",
                RESOURCE_DIR"/cats/038/Animation/attack16.png",
                RESOURCE_DIR"/cats/038/Animation/attack17.png",
                RESOURCE_DIR"/cats/038/Animation/attack17.png"// for padding
            });

        cat_gang.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/038/Animation/hitback.png"});
    }
    {
        auto &leaf_cat = s_anime[static_cast<size_t>(CatType::LEAF_CAT)];

        leaf_cat.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/039/Animation/idle.png"
            });

        leaf_cat.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/039/Animation/walk0.png",
                RESOURCE_DIR"/cats/039/Animation/walk1.png",
                RESOURCE_DIR"/cats/039/Animation/walk2.png",
                RESOURCE_DIR"/cats/039/Animation/walk3.png",
                RESOURCE_DIR"/cats/039/Animation/walk4.png",
                RESOURCE_DIR"/cats/039/Animation/walk5.png"});

        leaf_cat.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/039/Animation/attack0.png",
                RESOURCE_DIR"/cats/039/Animation/attack1.png",
                RESOURCE_DIR"/cats/039/Animation/attack2.png",
                RESOURCE_DIR"/cats/039/Animation/attack3.png",
                RESOURCE_DIR"/cats/039/Animation/attack4.png",
                RESOURCE_DIR"/cats/039/Animation/attack5.png",
                RESOURCE_DIR"/cats/039/Animation/attack5.png"// for padding
            });

        leaf_cat.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/039/Animation/hitback.png"});
    }
    {
        auto &miss_moneko = s_anime[static_cast<size_t>(CatType::MISS_MONEKO)];

        miss_moneko.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/040/Animation/idle.png"
            });

        miss_moneko.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/040/Animation/walk0.png",
                RESOURCE_DIR"/cats/040/Animation/walk1.png",
                RESOURCE_DIR"/cats/040/Animation/walk10.png",
                RESOURCE_DIR"/cats/040/Animation/walk2.png",
                RESOURCE_DIR"/cats/040/Animation/walk3.png",
                RESOURCE_DIR"/cats/040/Animation/walk4.png",
                RESOURCE_DIR"/cats/040/Animation/walk5.png",
                RESOURCE_DIR"/cats/040/Animation/walk6.png",
                RESOURCE_DIR"/cats/040/Animation/walk7.png",
                RESOURCE_DIR"/cats/040/Animation/walk8.png",
                RESOURCE_DIR"/cats/040/Animation/walk9.png"
            });

        miss_moneko.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/040/Animation/attack0.png",
                RESOURCE_DIR"/cats/040/Animation/attack1.png",
                RESOURCE_DIR"/cats/040/Animation/attack2.png",
                RESOURCE_DIR"/cats/040/Animation/attack3.png",
                RESOURCE_DIR"/cats/040/Animation/attack4.png",
                RESOURCE_DIR"/cats/040/Animation/attack5.png",
                RESOURCE_DIR"/cats/040/Animation/attack6.png",
                RESOURCE_DIR"/cats/040/Animation/attack7.png",
                RESOURCE_DIR"/cats/040/Animation/attack8.png",
                RESOURCE_DIR"/cats/040/Animation/attack9.png",
                RESOURCE_DIR"/cats/040/Animation/attack10.png",
                RESOURCE_DIR"/cats/040/Animation/attack11.png",
                RESOURCE_DIR"/cats/040/Animation/attack12.png",
                RESOURCE_DIR"/cats/040/Animation/attack13.png",
                RESOURCE_DIR"/cats/040/Animation/attack14.png",
                RESOURCE_DIR"/cats/040/Animation/attack15.png",
                RESOURCE_DIR"/cats/040/Animation/attack16.png",
                RESOURCE_DIR"/cats/040/Animation/attack17.png",
                RESOURCE_DIR"/cats/040/Animation/attack18.png",
                RESOURCE_DIR"/cats/040/Animation/attack19.png",
                RESOURCE_DIR"/cats/040/Animation/attack20.png",
                RESOURCE_DIR"/cats/040/Animation/attack20.png" // for padding
            });

        miss_moneko.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/040/Animation/hitback.png"});
    }
    //---------------------------------
    {
        auto &biker_cat = s_anime[static_cast<size_t>(CatType::BIKER_CAT)];

        biker_cat.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/041/Animation/idle.png"
            });

        biker_cat.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/041/Animation/walk0.png",
                RESOURCE_DIR"/cats/041/Animation/walk1.png",
                RESOURCE_DIR"/cats/041/Animation/walk2.png",
                RESOURCE_DIR"/cats/041/Animation/walk3.png",
                RESOURCE_DIR"/cats/041/Animation/walk4.png",
                RESOURCE_DIR"/cats/041/Animation/walk5.png"
            });

        biker_cat.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/041/Animation/attack0.png",
                RESOURCE_DIR"/cats/041/Animation/attack1.png",
                RESOURCE_DIR"/cats/041/Animation/attack2.png",
                RESOURCE_DIR"/cats/041/Animation/attack3.png",
                RESOURCE_DIR"/cats/041/Animation/attack4.png",
                RESOURCE_DIR"/cats/041/Animation/attack5.png",
                RESOURCE_DIR"/cats/041/Animation/attack6.png",
                RESOURCE_DIR"/cats/041/Animation/attack7.png",
                RESOURCE_DIR"/cats/041/Animation/attack8.png",
                RESOURCE_DIR"/cats/041/Animation/attack9.png",
                RESOURCE_DIR"/cats/041/Animation/attack10.png",
                RESOURCE_DIR"/cats/041/Animation/attack11.png",
                RESOURCE_DIR"/cats/041/Animation/attack12.png",
                RESOURCE_DIR"/cats/041/Animation/attack13.png",
                RESOURCE_DIR"/cats/041/Animation/attack14.png",
                RESOURCE_DIR"/cats/041/Animation/attack15.png",
                RESOURCE_DIR"/cats/041/Animation/attack16.png",
                RESOURCE_DIR"/cats/041/Animation/attack17.png",
                RESOURCE_DIR"/cats/041/Animation/attack18.png",
                RESOURCE_DIR"/cats/041/Animation/attack18.png"
                 // for padding
            });

        biker_cat.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/041/Animation/hitback.png"});
    }
    {
        auto &ninja_frog_cat = s_anime[static_cast<size_t>(CatType::NINJA_FROG_CAT)];

        ninja_frog_cat.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/042/Animation/idle.png"
            });

        ninja_frog_cat.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/042/Animation/walk0.png",
                RESOURCE_DIR"/cats/042/Animation/walk1.png",
                RESOURCE_DIR"/cats/042/Animation/walk2.png",
                RESOURCE_DIR"/cats/042/Animation/walk3.png",
                RESOURCE_DIR"/cats/042/Animation/walk4.png"
            });

        ninja_frog_cat.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/042/Animation/attack0.png",
                RESOURCE_DIR"/cats/042/Animation/attack1.png",
                RESOURCE_DIR"/cats/042/Animation/attack2.png",
                RESOURCE_DIR"/cats/042/Animation/attack3.png",
                RESOURCE_DIR"/cats/042/Animation/attack4.png",
                RESOURCE_DIR"/cats/042/Animation/attack5.png",
                RESOURCE_DIR"/cats/042/Animation/attack6.png",
                RESOURCE_DIR"/cats/042/Animation/attack6.png"
                // for padding
            });

        ninja_frog_cat.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/042/Animation/hitback.png"});
    }
    {
        auto &devil_cat = s_anime[static_cast<size_t>(CatType::DEVIL_CAT)];

        devil_cat.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/043/Animation/idle.png"
            });

        devil_cat.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/043/Animation/walk0.png",
                RESOURCE_DIR"/cats/043/Animation/walk1.png",
                RESOURCE_DIR"/cats/043/Animation/walk2.png",
                RESOURCE_DIR"/cats/043/Animation/walk3.png",
                RESOURCE_DIR"/cats/043/Animation/walk4.png"
            });

        devil_cat.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/043/Animation/attack0.png",
                RESOURCE_DIR"/cats/043/Animation/attack1.png",
                RESOURCE_DIR"/cats/043/Animation/attack2.png",
                RESOURCE_DIR"/cats/043/Animation/attack3.png",
                RESOURCE_DIR"/cats/043/Animation/attack4.png",
                RESOURCE_DIR"/cats/043/Animation/attack5.png",
                RESOURCE_DIR"/cats/043/Animation/attack6.png",
                RESOURCE_DIR"/cats/043/Animation/attack7.png",
                RESOURCE_DIR"/cats/043/Animation/attack8.png",
                RESOURCE_DIR"/cats/043/Animation/attack9.png",
                RESOURCE_DIR"/cats/043/Animation/attack10.png",
                RESOURCE_DIR"/cats/043/Animation/attack11.png",
                RESOURCE_DIR"/cats/043/Animation/attack12.png",
                RESOURCE_DIR"/cats/043/Animation/attack12.png"
                // for padding
            });

        devil_cat.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/043/Animation/hitback.png"});
    }
    {
        auto &knight_cat = s_anime[static_cast<size_t>(CatType::KNIGHT_CAT)];

        knight_cat.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR
                                               "/cats/044/Animation/idle.png"});

        knight_cat.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/044/Animation/walk0.png",
                RESOURCE_DIR "/cats/044/Animation/walk1.png",
                RESOURCE_DIR "/cats/044/Animation/walk2.png",
                RESOURCE_DIR "/cats/044/Animation/walk3.png",
                RESOURCE_DIR "/cats/044/Animation/walk4.png",
                RESOURCE_DIR "/cats/044/Animation/walk5.png",
                RESOURCE_DIR "/cats/044/Animation/walk6.png",
                RESOURCE_DIR "/cats/044/Animation/walk7.png"});

        knight_cat.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/044/Animation/attack0.png",
                RESOURCE_DIR "/cats/044/Animation/attack1.png",
                RESOURCE_DIR "/cats/044/Animation/attack2.png",
                RESOURCE_DIR "/cats/044/Animation/attack3.png",
                RESOURCE_DIR "/cats/044/Animation/attack4.png",
                RESOURCE_DIR "/cats/044/Animation/attack4.png"
                // for padding
            });

        knight_cat.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/cats/044/Animation/hitback.png"});
    }
    {
        auto &madame_sumo_cat = s_anime[static_cast<size_t>(CatType::MADAME_SUMO_CAT)];

        madame_sumo_cat.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/045/Animation/idle.png"
            });

        madame_sumo_cat.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/045/Animation/walk0.png",
                RESOURCE_DIR"/cats/045/Animation/walk1.png",
                RESOURCE_DIR"/cats/045/Animation/walk2.png",
                RESOURCE_DIR"/cats/045/Animation/walk3.png",
                RESOURCE_DIR"/cats/045/Animation/walk4.png"
            });

        madame_sumo_cat.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/045/Animation/attack0.png",
                RESOURCE_DIR"/cats/045/Animation/attack1.png",
                RESOURCE_DIR"/cats/045/Animation/attack2.png",
                RESOURCE_DIR"/cats/045/Animation/attack3.png",
                RESOURCE_DIR"/cats/045/Animation/attack4.png",
                RESOURCE_DIR"/cats/045/Animation/attack5.png",
                RESOURCE_DIR"/cats/045/Animation/attack6.png",
                RESOURCE_DIR"/cats/045/Animation/attack7.png",
                RESOURCE_DIR"/cats/045/Animation/attack8.png",
                RESOURCE_DIR"/cats/045/Animation/attack9.png",
                RESOURCE_DIR"/cats/045/Animation/attack10.png",
                RESOURCE_DIR"/cats/045/Animation/attack11.png",
                RESOURCE_DIR"/cats/045/Animation/attack11.png"
                // for padding
            });

        madame_sumo_cat.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/045/Animation/hitback.png"});
    }
    {
        auto &samba_cat = s_anime[static_cast<size_t>(CatType::SAMBA_CAT)];

        samba_cat.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/046/Animation/idle.png"
            });

        samba_cat.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/046/Animation/walk0.png",
                RESOURCE_DIR"/cats/046/Animation/walk1.png",
                RESOURCE_DIR"/cats/046/Animation/walk2.png",
                RESOURCE_DIR"/cats/046/Animation/walk3.png",
                RESOURCE_DIR"/cats/046/Animation/walk4.png",
                RESOURCE_DIR"/cats/046/Animation/walk5.png",
                RESOURCE_DIR"/cats/046/Animation/walk6.png",
                RESOURCE_DIR"/cats/046/Animation/walk7.png",
                RESOURCE_DIR"/cats/046/Animation/walk8.png"
            });

        samba_cat.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/046/Animation/attack0.png",
                RESOURCE_DIR"/cats/046/Animation/attack1.png",
                RESOURCE_DIR"/cats/046/Animation/attack2.png",
                RESOURCE_DIR"/cats/046/Animation/attack3.png",
                RESOURCE_DIR"/cats/046/Animation/attack4.png",
                RESOURCE_DIR"/cats/046/Animation/attack5.png",
                RESOURCE_DIR"/cats/046/Animation/attack6.png",
                RESOURCE_DIR"/cats/046/Animation/attack7.png",
                RESOURCE_DIR"/cats/046/Animation/attack8.png",
                RESOURCE_DIR"/cats/046/Animation/attack9.png",
                RESOURCE_DIR"/cats/046/Animation/attack9.png"
                // for padding
            });

        samba_cat.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/046/Animation/hitback.png"});
    }
    {
        auto &tights_cat = s_anime[static_cast<size_t>(CatType::TIGHTS_CAT)];

        tights_cat.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/047/Animation/idle.png"
            });

        tights_cat.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/047/Animation/walk0.png",
                RESOURCE_DIR"/cats/047/Animation/walk1.png",
                RESOURCE_DIR"/cats/047/Animation/walk2.png",
                RESOURCE_DIR"/cats/047/Animation/walk3.png",
                RESOURCE_DIR"/cats/047/Animation/walk4.png",
                RESOURCE_DIR"/cats/047/Animation/walk5.png",
                RESOURCE_DIR"/cats/047/Animation/walk6.png"
            });

        tights_cat.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/047/Animation/attack0.png",
                RESOURCE_DIR"/cats/047/Animation/attack1.png",
                RESOURCE_DIR"/cats/047/Animation/attack2.png",
                RESOURCE_DIR"/cats/047/Animation/attack3.png",
                RESOURCE_DIR"/cats/047/Animation/attack4.png",
                RESOURCE_DIR"/cats/047/Animation/attack4.png"
                // for padding
            });

        tights_cat.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{
                RESOURCE_DIR"/cats/047/Animation/hitback.png"});
    }
}

const Cat::Animation CatAnimeResource::Get(const CatType type){
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
