#ifndef ENEMYATTR_HPP
#define ENEMYATTR_HPP

#pragma once

#include <cstdint>

enum EnemyAttr : uint16_t {
    WHITE = 0x01,
    RED = 0x02,
    FLOATING = 0x04,
    BLACK = 0x08
};
#endif //ENEMYATTR_HPP
