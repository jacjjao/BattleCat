#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "EnemyAttr.hpp"
#include "EntityStats.hpp"
#include "Entity.hpp"
#include "RandomGenerator.hpp"
#include <array>

enum class EnemyType : size_t {
    ENEMY_TOWER = 0,
    DOGE,
    SNACHE,
    THOSE_GUYS,
    HIPPOE,
    PIGGE,
    ENEMY_TYPE_COUNT
};
static_assert(std::is_same_v<std::underlying_type_t<EnemyType>, size_t>);

class Enemy : public Entity {
public:
    struct Animation {
        std::unique_ptr<AnimatedGameObject> walk;
        std::unique_ptr<AnimatedGameObject> attack;
        std::unique_ptr<AnimatedGameObject> idle;
        std::unique_ptr<AnimatedGameObject> knockback;
    };

    explicit Enemy(EnemyType type);

    void StartAttack();

    void Draw(Util::Transform trans, Animation &anime);

    void UpdateTimer(double dt);

    void Walk(float dt);

    void DealDamage(Entity &e) override;

    [[nodiscard]]
    EnemyType GetEnemyType() const;

    bool OnAttack();

    void SetStatsModifier(float modifier);

    void SetY(float low, float high);

private:
    void Attack();
    void CoolDownComplete();
    void OnHitBack() override;

    inline static RandomFloatGenerator s_Random;

    [[nodiscard]]
    HitBox ToWorldSpace(HitBox hitbox) const override;

    EntityState m_PrevDrawState;

    float m_TargetY = 0.0f;

    double hb_vel_y = 0.0; // hitback velocity - y axis
    double hb_dy = 0.0;
    int land = 0;

    EnemyType m_Type;
    bool m_OnAttack = false;
};
//--------------------------------------------------------

namespace EnemyStats {

    inline const EntityStats EnemyTower = [] {
        EntityStats stats;
        stats.health = 1000;
        stats.kb = 1;
        stats.single_target = true;
        stats.attr = std::nullopt;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "EnemyTower";
#endif
        return stats;
    }();

    inline const EntityStats Doge = [] { // tmp
        EntityStats stats;
        stats.health = 100;
        stats.damage = 20;
        stats.range = 140;
        stats.kb = 3;
        stats.speed = 30;
        stats.single_target = true;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 0.96;
        stats.recharge_time = 2000;
        stats.cost = 75;
        stats.det_box = {-58, 0};
        stats.hit_box = {-58, 0};
        stats.attr = EnemyAttr::WHITE;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "Doge";
#endif
        return stats;
    }();

    inline const EntityStats Snache = [] { // tmp
        EntityStats stats;
        stats.health = 100;
        stats.damage = 15;
        stats.range = 110;
        stats.kb = 3;
        stats.speed = 48;
        stats.single_target = true;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 1.00;
        stats.recharge_time = 2000;
        stats.cost = 150;
        stats.det_box = {-58, 0};
        stats.hit_box = {-58, 0};
        stats.attr = EnemyAttr::WHITE;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "Snache";
#endif
        return stats;
    }();

    inline const EntityStats ThoseGuys = [] { // tmp
        EntityStats stats;
        stats.health = 200;
        stats.damage = 20;
        stats.range = 110;
        stats.kb = 1;
        stats.speed = 60;
        stats.single_target = true;
        stats.atk_prep_time = 0.33;
        stats.atk_cool_down = 0.67;
        stats.recharge_time = 2000;
        stats.cost = 150;
        stats.det_box = {-58, 0};
        stats.hit_box = {-58, 0};
        stats.attr = EnemyAttr::WHITE;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "ThoseGuys";
#endif
        return stats;
    }();

    inline const EntityStats Hippoe = [] { // tmp
        EntityStats stats;
        stats.health = 1000;
        stats.damage = 100;
        stats.range = 160;
        stats.kb = 1;
        stats.speed = 24;
        stats.single_target = false;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 2.00;
        stats.recharge_time = 2000;
        stats.cost = 2000;
        stats.det_box = {-58, 0};
        stats.hit_box = {-58, 0};
        stats.attr = EnemyAttr::WHITE;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "Hippoe";
#endif
        return stats;
    }();

    inline const EntityStats Pigge = [] { // tmp
        EntityStats stats;
        stats.health = 1500;
        stats.damage = 120;
        stats.range = 150;
        stats.kb = 2;
        stats.speed = 30;
        stats.single_target = false;
        stats.atk_prep_time = 0.47;
        stats.atk_cool_down = 2.00;
        stats.recharge_time = 2000;
        stats.cost = 2000;
        stats.det_box = {-58, 0};
        stats.hit_box = {-58, 0};
        stats.attr = EnemyAttr::RED;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "Pigge";
#endif
        return stats;
    }();

    inline const EntityStats JackiePeng = [] { // tmp
        EntityStats stats;
        stats.health = 1300;
        stats.damage = 80;
        stats.range = 150;
        stats.kb = 3;
        stats.speed = 60;
        stats.single_target = true;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 0.33;
        stats.recharge_time = 2000;
        stats.cost = 1500;
        stats.det_box = {-58, 0};
        stats.hit_box = {-58, 0};
        stats.attr = EnemyAttr::WHITE;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "JackiePeng";
#endif
            return stats;
    }();

    inline const EntityStats Gory = [] { // tmp
        EntityStats stats;
        stats.health = 1000;
        stats.damage = 80;
        stats.range = 150;
        stats.kb = 3;
        stats.speed = 90;
        stats.single_target = false;
        stats.atk_prep_time = 0.27;
        stats.atk_cool_down = 0.067;
        stats.recharge_time = 2000;
        stats.cost = 2500;
        stats.det_box = {-58, 0};
        stats.hit_box = {-58, 0};
        stats.attr = EnemyAttr::WHITE;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "Gory";
#endif
        return stats;
    }();

    inline const EntityStats BaaBaa = [] { // tmp
        EntityStats stats;
        stats.health = 500;
        stats.damage = 50;
        stats.range = 110;
        stats.kb = 3;
        stats.speed = 42;
        stats.single_target = true;
        stats.atk_prep_time = 0.46;
        stats.atk_cool_down = 1.33;
        stats.recharge_time = 2000;
        stats.cost = 1000;
        stats.det_box = {-58, 0};
        stats.hit_box = {-58, 0};
        stats.attr = EnemyAttr::WHITE;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "BaaBaa";
#endif
        return stats;
    }();

    inline const EntityStats SirSeal = [] { // tmp
        EntityStats stats;
        stats.health = 2500;
        stats.damage = 150;
        stats.range = 170;
        stats.kb = 1;
        stats.speed = 60;
        stats.single_target = false;
        stats.atk_prep_time = 0.26;
        stats.atk_cool_down = 0.53;
        stats.recharge_time = 2000;
        stats.cost = 5000;
        stats.det_box = {-58, 0};
        stats.hit_box = {-58, 0};
        stats.attr = EnemyAttr::RED;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "SirSeal";
#endif
        return stats;
    }();

    inline const EntityStats Leboin = [] { // tmp
        EntityStats stats;
        stats.health = 4000;
        stats.damage = 654;
        stats.range = 400;
        stats.kb = 1;
        stats.speed = 24;
        stats.single_target = false;
        stats.atk_prep_time = 0.26;
        stats.atk_cool_down = 6.00;
        stats.recharge_time = 2000;
        stats.cost = 10000;
        stats.det_box = {-58, 0};
        stats.hit_box = {-58, 0};
        stats.attr = EnemyAttr::WHITE;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "Leboin";
#endif
        return stats;
    }();

    inline const EntityStats KangRoo = [] { // tmp
        EntityStats stats;
        stats.health = 4000;
        stats.damage = 213;
        stats.range = 160;
        stats.kb = 1;
        stats.speed = 120;
        stats.single_target = true;
        stats.atk_prep_time = 0.067;
        stats.atk_cool_down = 0.067;
        stats.recharge_time = 2000;
        stats.cost = 10000;
        stats.det_box = {-58, 0};
        stats.hit_box = {-58, 0};
        stats.attr = EnemyAttr::WHITE;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "KangRoo";
#endif
        return stats;
    }();

    inline const EntityStats OneHorn = [] { // tmp
        EntityStats stats;
        stats.health = 15000;
        stats.damage = 500;
        stats.range = 40;
        stats.kb = 1;
        stats.speed = 18;
        stats.single_target = false;
        stats.atk_prep_time = 0.26;
        stats.atk_cool_down = 0;
        stats.recharge_time = 2000;
        stats.cost = 20000;
        stats.det_box = {-58, 0};
        stats.hit_box = {-58, 0};
        stats.attr = EnemyAttr::RED;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "OneHorn";
#endif
        return stats;
    }();

    inline const std::array<EntityStats,
                            static_cast<size_t>(EnemyType::ENEMY_TYPE_COUNT)>
        Stats = {EnemyStats::EnemyTower, EnemyStats::Doge, EnemyStats::Snache,
                 EnemyStats::ThoseGuys, EnemyStats::Hippoe, EnemyStats::Pigge};

}

//----------------------------------------------------------------------
namespace EnemyAnime{
    inline Enemy::Animation Tower() {
        Enemy::Animation a;
        a.idle = std::make_unique<AnimatedGameObject>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/stages/ec045_tw.png"});
        return a;
    }

    inline Enemy::Animation Doge() {
        auto walk = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/enemys/000/Animation/walk0.png",
            RESOURCE_DIR "/enemys/000/Animation/walk1.png"
        });
        walk->SetInterval(300); // ms
        walk->SetLooping(true);

        auto attack = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/enemys/000/Animation/attack0.png",
            RESOURCE_DIR "/enemys/000/Animation/attack1.png",
            RESOURCE_DIR "/enemys/000/Animation/attack1.png" // for padding
        });
        attack->SetInterval(EnemyStats::Doge.atk_prep_time * 1000.0 / 2.0);
        attack->SetLooping(false);

        auto idle = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/enemys/000/Animation/idle.png"
        });

        auto knockback = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{RESOURCE_DIR "/enemys/000/Animation/knockback.png"});

        Enemy::Animation a;
        a.walk = std::move(walk);
        a.attack = std::move(attack);
        a.idle = std::move(idle);
        a.knockback = std::move(knockback);
        return a;
    }

    inline Enemy::Animation Snache() {
        auto walk = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/enemys/001/Animation/walk0.png",
            RESOURCE_DIR "/enemys/001/Animation/walk1.png"
        });
        walk->SetInterval(300); // ms
        walk->SetLooping(true);

        auto attack = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/enemys/001/Animation/attack0.png",
            RESOURCE_DIR "/enemys/001/Animation/attack1.png",
            RESOURCE_DIR "/enemys/001/Animation/attack2.png",
            RESOURCE_DIR "/enemys/001/Animation/attack3.png",
            RESOURCE_DIR "/enemys/001/Animation/attack3.png" // for padding
        });
        attack->SetInterval(EnemyStats::Snache.atk_prep_time * 1000.0 / 4.0);
        attack->SetLooping(false);

        auto idle = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/enemys/001/Animation/idle.png"
        });

        auto knockback = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{RESOURCE_DIR "/enemys/001/Animation/knockback.png"});

        Enemy::Animation a;
        a.walk = std::move(walk);
        a.attack = std::move(attack);
        a.idle = std::move(idle);
        a.knockback = std::move(knockback);
        return a;
    }

    inline Enemy::Animation ThoseGuys() {
        auto walk = std::make_unique<AnimatedGameObject>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/002/Animation/walk0.png",
                RESOURCE_DIR "/enemys/002/Animation/walk1.png",
                RESOURCE_DIR "/enemys/002/Animation/walk2.png",
                RESOURCE_DIR "/enemys/002/Animation/walk3.png",
                RESOURCE_DIR "/enemys/002/Animation/walk4.png",
                RESOURCE_DIR "/enemys/002/Animation/walk5.png"});
        walk->SetInterval(300); // ms
        walk->SetLooping(true);

        auto attack = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
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
        attack->SetInterval(EnemyStats::ThoseGuys.atk_prep_time * 1000.0 / 7.0);
        attack->SetLooping(false);

        auto idle = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/enemys/002/Animation/idle.png"
        });

        auto knockback = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{RESOURCE_DIR "/enemys/002/Animation/knockback.png"});

        Enemy::Animation a;
        a.walk = std::move(walk);
        a.attack = std::move(attack);
        a.idle = std::move(idle);
        a.knockback = std::move(knockback);
        return a;
    }

    inline Enemy::Animation Hippoe() {
        auto walk = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/enemys/003/Animation/walk0.png",
            RESOURCE_DIR "/enemys/003/Animation/walk1.png"
        });
        walk->SetInterval(300); // ms
        walk->SetLooping(true);

        auto attack = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/enemys/003/Animation/attack0.png",
            RESOURCE_DIR "/enemys/003/Animation/attack1.png",
            RESOURCE_DIR "/enemys/003/Animation/attack2.png",
            RESOURCE_DIR "/enemys/003/Animation/attack2.png" // for padding
        });
        attack->SetInterval(EnemyStats::Hippoe.atk_prep_time * 1000.0 / 3.0);
        attack->SetLooping(false);

        auto idle = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/enemys/003/Animation/idle.png"
        });

        auto knockback = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{RESOURCE_DIR "/enemys/003/Animation/knockback.png"});

        Enemy::Animation a;
        a.walk = std::move(walk);
        a.attack = std::move(attack);
        a.idle = std::move(idle);
        a.knockback = std::move(knockback);
        return a;
    }

    inline Enemy::Animation Pigge() {
        auto walk = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/enemys/004/Animation/walk0.png",
            RESOURCE_DIR "/enemys/004/Animation/walk1.png",
            RESOURCE_DIR "/enemys/004/Animation/walk3.png"
        });
        walk->SetInterval(300); // ms
        walk->SetLooping(true);

        auto attack = std::make_unique<AnimatedGameObject>(
            std::initializer_list<std::string>{
                RESOURCE_DIR "/enemys/004/Animation/attack0.png",
                RESOURCE_DIR "/enemys/004/Animation/attack1.png",
                RESOURCE_DIR "/enemys/004/Animation/attack2.png",
                RESOURCE_DIR "/enemys/004/Animation/attack3.png",
                RESOURCE_DIR "/enemys/004/Animation/attack4.png",
                RESOURCE_DIR "/enemys/004/Animation/attack5.png",
                RESOURCE_DIR "/enemys/004/Animation/attack5.png" // for padding
            });
        attack->SetInterval(EnemyStats::Pigge.atk_prep_time * 1000.0 / 3.0);
        attack->SetLooping(false);

        auto idle = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/enemys/004/Animation/idle.png"
        });

        auto knockback = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{RESOURCE_DIR "/enemys/004/Animation/attack0.png"});

        Enemy::Animation a;
        a.walk = std::move(walk);
        a.attack = std::move(attack);
        a.idle = std::move(idle);
        a.knockback = std::move(knockback);
        return a;
    }

    inline Enemy::Animation JackiePeng() {
        auto walk = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/walk0.png",
            RESOURCE_DIR "/cats/000/Animation/walk1.png"
        });
        walk->SetInterval(300); // ms
        walk->SetLooping(true);

        auto attack = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/attack_prev0.png",
            RESOURCE_DIR "/cats/000/Animation/attack_prev1.png",
            RESOURCE_DIR "/cats/000/Animation/attack_post.png",
            RESOURCE_DIR "/cats/000/Animation/attack_post.png" // for padding
        });
        attack->SetInterval(EnemyStats::JackiePeng.atk_prep_time * 1000.0 / 3.0);
        attack->SetLooping(false);

        auto idle = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/idle.png"
        });

        auto knockback = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{RESOURCE_DIR "/cats/000/Animation/knockback.png"});

        Enemy::Animation a;
        a.walk = std::move(walk);
        a.attack = std::move(attack);
        a.idle = std::move(idle);
        a.knockback = std::move(knockback);
        return a;
    }

    inline Enemy::Animation Gory() {
        auto walk = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/walk0.png",
            RESOURCE_DIR "/cats/000/Animation/walk1.png"
        });
        walk->SetInterval(300); // ms
        walk->SetLooping(true);

        auto attack = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/attack_prev0.png",
            RESOURCE_DIR "/cats/000/Animation/attack_prev1.png",
            RESOURCE_DIR "/cats/000/Animation/attack_post.png",
            RESOURCE_DIR "/cats/000/Animation/attack_post.png" // for padding
        });
        attack->SetInterval(EnemyStats::Gory.atk_prep_time * 1000.0 / 3.0);
        attack->SetLooping(false);

        auto idle = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/idle.png"
        });

        auto knockback = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{RESOURCE_DIR "/cats/000/Animation/knockback.png"});

        Enemy::Animation a;
        a.walk = std::move(walk);
        a.attack = std::move(attack);
        a.idle = std::move(idle);
        a.knockback = std::move(knockback);
        return a;
    }

    inline Enemy::Animation BaaBaa() {
        auto walk = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/walk0.png",
            RESOURCE_DIR "/cats/000/Animation/walk1.png"
        });
        walk->SetInterval(300); // ms
        walk->SetLooping(true);

        auto attack = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/attack_prev0.png",
            RESOURCE_DIR "/cats/000/Animation/attack_prev1.png",
            RESOURCE_DIR "/cats/000/Animation/attack_post.png",
            RESOURCE_DIR "/cats/000/Animation/attack_post.png" // for padding
        });
        attack->SetInterval(EnemyStats::BaaBaa.atk_prep_time * 1000.0 / 3.0);
        attack->SetLooping(false);

        auto idle = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/idle.png"
        });

        auto knockback = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{RESOURCE_DIR "/cats/000/Animation/knockback.png"});

        Enemy::Animation a;
        a.walk = std::move(walk);
        a.attack = std::move(attack);
        a.idle = std::move(idle);
        a.knockback = std::move(knockback);
        return a;
    }

    inline Enemy::Animation SirSeal() {
        auto walk = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/walk0.png",
            RESOURCE_DIR "/cats/000/Animation/walk1.png"
        });
        walk->SetInterval(300); // ms
        walk->SetLooping(true);

        auto attack = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/attack_prev0.png",
            RESOURCE_DIR "/cats/000/Animation/attack_prev1.png",
            RESOURCE_DIR "/cats/000/Animation/attack_post.png",
            RESOURCE_DIR "/cats/000/Animation/attack_post.png" // for padding
        });
        attack->SetInterval(EnemyStats::SirSeal.atk_prep_time * 1000.0 / 3.0);
        attack->SetLooping(false);

        auto idle = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/idle.png"
        });

        auto knockback = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{RESOURCE_DIR "/cats/000/Animation/knockback.png"});

        Enemy::Animation a;
        a.walk = std::move(walk);
        a.attack = std::move(attack);
        a.idle = std::move(idle);
        a.knockback = std::move(knockback);
        return a;
    }

    inline Enemy::Animation Leboin() {
        auto walk = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/walk0.png",
            RESOURCE_DIR "/cats/000/Animation/walk1.png"
        });
        walk->SetInterval(300); // ms
        walk->SetLooping(true);

        auto attack = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/attack_prev0.png",
            RESOURCE_DIR "/cats/000/Animation/attack_prev1.png",
            RESOURCE_DIR "/cats/000/Animation/attack_post.png",
            RESOURCE_DIR "/cats/000/Animation/attack_post.png" // for padding
        });
        attack->SetInterval(EnemyStats::Leboin.atk_prep_time * 1000.0 / 3.0);
        attack->SetLooping(false);

        auto idle = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/idle.png"
        });

        auto knockback = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{RESOURCE_DIR "/cats/000/Animation/knockback.png"});

        Enemy::Animation a;
        a.walk = std::move(walk);
        a.attack = std::move(attack);
        a.idle = std::move(idle);
        a.knockback = std::move(knockback);
        return a;
    }

    inline Enemy::Animation KangRoo() {
        auto walk = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/walk0.png",
            RESOURCE_DIR "/cats/000/Animation/walk1.png"
        });
        walk->SetInterval(300); // ms
        walk->SetLooping(true);

        auto attack = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/attack_prev0.png",
            RESOURCE_DIR "/cats/000/Animation/attack_prev1.png",
            RESOURCE_DIR "/cats/000/Animation/attack_post.png",
            RESOURCE_DIR "/cats/000/Animation/attack_post.png" // for padding
        });
        attack->SetInterval(EnemyStats::KangRoo.atk_prep_time * 1000.0 / 3.0);
        attack->SetLooping(false);

        auto idle = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/idle.png"
        });

        auto knockback = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{RESOURCE_DIR "/cats/000/Animation/knockback.png"});

        Enemy::Animation a;
        a.walk = std::move(walk);
        a.attack = std::move(attack);
        a.idle = std::move(idle);
        a.knockback = std::move(knockback);
        return a;
    }

    inline Enemy::Animation OneHorn() {
        auto walk = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/walk0.png",
            RESOURCE_DIR "/cats/000/Animation/walk1.png"
        });
        walk->SetInterval(300); // ms
        walk->SetLooping(true);

        auto attack = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/attack_prev0.png",
            RESOURCE_DIR "/cats/000/Animation/attack_prev1.png",
            RESOURCE_DIR "/cats/000/Animation/attack_post.png",
            RESOURCE_DIR "/cats/000/Animation/attack_post.png" // for padding
        });
        attack->SetInterval(EnemyStats::OneHorn.atk_prep_time * 1000.0 / 3.0);
        attack->SetLooping(false);

        auto idle = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{
            RESOURCE_DIR "/cats/000/Animation/idle.png"
        });

        auto knockback = std::make_unique<AnimatedGameObject>(std::initializer_list<std::string>{RESOURCE_DIR "/cats/000/Animation/knockback.png"});

        Enemy::Animation a;
        a.walk = std::move(walk);
        a.attack = std::move(attack);
        a.idle = std::move(idle);
        a.knockback = std::move(knockback);
        return a;
    }
































































}
#endif //ENEMY_HPP
