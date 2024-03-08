#pragma once

#include <optional>
#include "EnemyAttr.hpp"

enum class EntityState { WALK, ON_ATTACK, ATTACK_COOLDOWN, KNOCK_BACK };

struct HitBox {
    int low;
    int high;
};

struct EntityStats {
    int health = 0;
    int damage = 0;
    int range = 0;
    int kb = 0;
    bool single_target = true;
    int speed = 0;
    int atk_prep_time = 0;
    int atk_cool_down = 0;
    int last_atk_time = 0;
    int recharge_time = 0;
    int cost = 0;
    HitBox det_box;
    HitBox hit_box;
    std::optional<EnemyAttr> attr;
};
