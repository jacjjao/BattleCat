#pragma once

#include <cstdint>

enum EnemyAttr : uint16_t {
    NIL = 0x00, // cats only
    WHITE = 0x01,
    RED = 0x02,
    FLOATING = 0x04,
    BLACK = 0x08
};
