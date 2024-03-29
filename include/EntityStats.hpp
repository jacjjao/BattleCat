#ifndef ENTITYSTATS_HPP
#define ENTITYSTATS_HPP

#include <optional>
#include <string>
#include <vector>
#include "EnemyAttr.hpp"

enum class EntityState { WALK, ATTACK, IDLE, HITBACK };

struct HitBox {
    float low;
    float high;
};

struct EntityStats {
    int health = 0;
    int damage = 0;
    int range = 0;
    int kb = 0;
    int speed = 0;
    bool single_target = true;
    double atk_prep_time = 0; // seconds
    double atk_cool_down = 0; // seconds
    int recharge_time = 0;
    int cost = 0;
    HitBox det_box; // detect or hurt box
    HitBox hit_box;
    std::optional<EnemyAttr> attr;
    int base_level = 0;
    int health_diff = 0;
    int damage_diff = 0;
    std::vector<EnemyAttr> strong;
#ifdef ENABLE_BATTLE_LOG
    std::string_view name = "Unknow";
#endif // ENABLE_BATTLE_LOG
};

#ifdef ENABLE_BATTLE_LOG

#include <string>

inline std::string EntityStateToString(EntityState state) {
    switch (state) { 
    case EntityState::WALK:
        return "WALK";

    case EntityState::ATTACK:
        return "ATTACK";

    case EntityState::IDLE:
        return "IDLE";

    case EntityState::HITBACK:
        return "HITBACK";
    }
    return "UNKNOW";
}

#endif // ENABLE_BATTLE_LOG
#endif // ENTITYSTATS_HPP
