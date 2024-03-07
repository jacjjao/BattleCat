#pragma once

#include "EnemyAttr.hpp"
#include "EntityStats.hpp"
#include <functional>

enum class CatType {
    CAT = 0
};
static_assert(std::is_same_v<std::underlying_type_t<CatType>, size_t>);

struct CatStatsMat {
    std::array<int, 10> health;
    std::array<int, 10> damage;
};

class Cat {
public:
    Cat(const EntityStats &stats, CatType type, int level,
        std::function<void(const Cat&)> hit_callback);

    void StartAttack();

    void GetHit(int damage, EnemyAttr attr);

    void Update();

    int GetDamage(EnemyAttr attr) const;

    CatType GetCatType() const;

    HitBox GetHitBox() const;

private:
    CatType m_Type;
    EntityStats m_Stats;
    EntityState m_State = EntityState::WALK;
};

namespace CatStats {

    inline constexpr EntityStats Cat = {.health = 250,
                                    .damage = 20,
                                    .range = 140,
                                    .kb = 3,
                                    .speed = 10,
                                    .single_target = true,
                                    .atk_prep_time = 270,
                                    .atk_cool_down = 1230,
                                    .recharge_time = 2000,
                                    .cost = 75,
                                    .det_box = {.low = 0, .high = 10},
                                    .attr = std::nullopt};

} // CatStats
