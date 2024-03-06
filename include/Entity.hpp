#pragma once

#include <glm/vec2.hpp>

struct Entity {
    enum class State {
        WALK,
        ATTACK,
        KNOCK_BACK
    };

    int health = 0;
    int move_speed = 0;
    int atk = 0;
    int atk_prep_time = 0;
    int atk_cool_Down = 0;
    int last_atk_time = 0;
    State state = State::WALK;
    int posX = 0;
};
