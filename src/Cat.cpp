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
    LoadResource();
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

void Cat::LoadResource() {
    switch (m_Type) { 
    case CatType::CAT_TOWER:
        m_Anime = CatAnime::Tower();
        break;

    case CatType::CAT:
        m_Anime = CatAnime::Cat();
        break;
    }
}

void CatAnimeResource::Init() {
    s_anime.resize(static_cast<size_t>(CatType::CAT_TYPE_COUNT));

    {
        auto &tower = s_anime[static_cast<size_t>(CatType::CAT_TOWER)];
        tower.idle = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR
                                               "/stages/ec000_tw.png"});
        tower.walk = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR
                                               "/stages/ec000_tw.png"});
        tower.attack = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR
                                               "/stages/ec000_tw.png"});
        tower.knockback = std::make_unique<SharedRc::Animation>(
            std::initializer_list<std::string>{RESOURCE_DIR
                                               "/stages/ec000_tw.png"});
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
