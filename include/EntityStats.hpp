#pragma once

#include <optional>
#include <string>
#include "EnemyAttr.hpp"

enum class EntityState { WALK, ON_ATTACK, ATTACK_COOLDOWN, KNOCK_BACK };

struct HitBox {
    float low;
    float high;
};

struct EntityStats {
    int health = 0;
    int damage = 0;
    int range = 0;
    int kb = 0;
    bool single_target = true;
    int speed = 0;
    double atk_prep_time = 0; // seconds
    double atk_cool_down = 0; // seconds
    int recharge_time = 0;
    int cost = 0;
    HitBox det_box; // detect or hurt box
    HitBox hit_box;
    std::optional<EnemyAttr> attr;
#ifdef ENABLE_BATTLE_LOG
    std::string name = "Unknow";
#endif
};
