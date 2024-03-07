#pragma once

#include <optional>
#include "EnemyAttr.hpp"

enum class EntityState { WALK, ON_ATTACK, ATTACK_COOLDOWN, KNOCK_BACK };

struct EntityStats {
    int health = 0;
    int damage = 0;
    int range = 0;
    int kb = 0;
    bool single_target = true;
    int move_speed = 0;
    int atk_prep_time = 0;
    int atk_cool_down = 0;
    int last_atk_time = 0;
    int recharge_time = 0;
    int cost = 0;
    std::optional<EnemyAttr> attr;
};
