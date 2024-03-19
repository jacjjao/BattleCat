#ifndef ENEMY_HPP
#define ENEMY_HPP

#pragma once

#include "EnemyAttr.hpp"
#include "EntityStats.hpp"
#include "Entity.hpp"
#include <array>

enum class EnemyType : size_t {
    DOGE = 0,
    ENEMY_TYPE_COUNT
};
static_assert(std::is_same_v<std::underlying_type_t<EnemyType>, size_t>);

class Enemy : public Entity {
public:
    Enemy(EnemyType type, float pos,
          std::function<void(Enemy &)> atk_callback);

    void StartAttack();

    void Draw(Util::Image &image) const override;

    void Update();

    void Walk(float dt);

    void DealDamage(Entity &e) override;

    EnemyType GetEnemyType() const;

    Enemy(Enemy &&other) noexcept;
    Enemy &operator=(Enemy &&other) noexcept;

private:
    void SetCallbacks();
    void Attack();
    void CoolDownComplete();

    HitBox ToWorldSpace(HitBox hitbox) const override;

    const std::function<void(Enemy &)> m_AtkCallback;
    EnemyType m_Type;
};

namespace EnemyStats {

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
        stats.det_box = {0, 10};
        stats.hit_box = {0, 10};
        stats.attr = EnemyAttr::WHITE;
#ifdef ENABLE_BATTLE_LOG
        stats.name = "Doge";
#endif
        return stats;
    }();

    inline const std::array<EntityStats,
                            static_cast<size_t>(EnemyType::ENEMY_TYPE_COUNT)>
        Stats = {EnemyStats::Doge};

}
#endif //ENEMY_HPP
