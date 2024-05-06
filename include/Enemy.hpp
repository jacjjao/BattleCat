#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "EnemyAttr.hpp"
#include "EntityStats.hpp"
#include "Entity.hpp"
#include <array>

enum class EnemyType : size_t {
    ENEMY_TOWER = 0,
    DOGE,
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

    void Draw(Util::Transform trans, Util::Image &image) const;

    void UpdateTimer(double dt);

    void Walk(float dt);

    void DealDamage(Entity &e) override;

    [[nodiscard]]
    EnemyType GetEnemyType() const;

    bool OnAttack();

    void SetStatsModifier(float modifier);

private:
    void Attack();
    void CoolDownComplete();

    [[nodiscard]]
    HitBox ToWorldSpace(HitBox hitbox) const override;

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
        stats.single_target = true;
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
        stats.single_target = true;
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

    inline const std::array<EntityStats,
                            static_cast<size_t>(EnemyType::ENEMY_TYPE_COUNT)>
        Stats = {EnemyStats::EnemyTower, EnemyStats::Doge};

}

//----------------------------------------------------------------------
namespace EnemyAnime{
    inline Enemy::Animation Doge() {
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
        attack->SetInterval(EnemyStats::Doge.atk_prep_time * 1000.0 / 3.0);
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

    inline Enemy::Animation Snache() {
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
        attack->SetInterval(EnemyStats::Snache.atk_prep_time * 1000.0 / 3.0);
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

    inline Enemy::Animation ThoseGuys() {
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
        attack->SetInterval(EnemyStats::ThoseGuys.atk_prep_time * 1000.0 / 3.0);
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

        inline Enemy::Animation Hippoe() {
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
        attack->SetInterval(EnemyStats::Hippoe.atk_prep_time * 1000.0 / 3.0);
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

    inline Enemy::Animation Pigge() {
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
        attack->SetInterval(EnemyStats::Pigge.atk_prep_time * 1000.0 / 3.0);
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
