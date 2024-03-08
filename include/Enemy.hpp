#pragma once

#include "EnemyAttr.hpp"
#include "EntityStats.hpp"
#include "Entity.hpp"

enum class EnemyType : size_t {
    DOGE = 0,
    ENEMY_TYPE_COUNT
};
static_assert(std::is_same_v<std::underlying_type_t<EnemyType>, size_t>);

class Enemy : public Entity {
public:
    void Draw(Util::Image &image) const override;

    void Update(float dt);

private:
    HitBox ToWorldSpace(HitBox hitbox) const override;
};

namespace EnemyStats {

    inline const EntityStats Doge = [] { // tmp
    EntityStats stats;
    stats.damage = 20;
    stats.range = 140;
    stats.kb = 3;
    stats.speed = 10;
    stats.single_target = true;
    stats.atk_prep_time = 0.27;
    stats.atk_cool_down = 1.23;
    stats.recharge_time = 2000;
    stats.cost = 75;
    stats.det_box = {0, 10};
    stats.hit_box = {0, 10};
    stats.attr = std::nullopt;
#ifdef ENABLE_BATTLE_LOG
    stats.name = "Doge";
#endif
    return stats;
}();

}
