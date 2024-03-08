#pragma once

#include "EnemyAttr.hpp"
#include "Entity.hpp"
#include <array>

enum class CatType : size_t {
    CAT = 0,
    CAT_TYPE_COUNT
};
static_assert(std::is_same_v<std::underlying_type_t<CatType>, size_t>);

struct CatStatsMat {
    std::array<int, 10> health;
    std::array<int, 10> damage;
};

class Cat : public Entity {
public:
    void Draw(Util::Image &image) const override;

    void Update(float dt);
};

namespace CatStats {

    inline const EntityStats Cat = [] {
    EntityStats stats;
    stats.damage = 20;
    stats.range = 140;
    stats.kb = 3;
    stats.speed = 10;
    stats.single_target = true;
    stats.atk_prep_time = 270;
    stats.atk_cool_down = 1230;
    stats.recharge_time = 2000;
    stats.cost = 75;
    stats.det_box = {0, 10};
    stats.hit_box = {0, 10};
    stats.attr = std::nullopt;
    return stats;
}();

} // CatStats
