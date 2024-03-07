#pragma once

#include "EnemyAttr.hpp"
#include "EntityStats.hpp"
#include <functional>

enum class EnemyType : size_t {
    DOGE = 0,
    ENEMY_TYPE_COUNT
};
static_assert(std::is_same_v<std::underlying_type_t<EnemyType>, size_t>);
