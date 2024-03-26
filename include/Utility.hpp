#ifndef UTILITY_HPP
#define UTILITY_HPP

#pragma once

#include <glm/vec2.hpp>
#include "Util/Keycode.hpp"

[[nodiscard]] 
bool PointInRect(glm::vec2 rect_tl_pos, glm::vec2 rect_size, glm::vec2 p);
[[nodiscard]]
bool PosInRange(glm::vec2 LTop,glm::vec2 RBottom,glm::vec2 pos);
/*[[nodiscard]]
bool fixKeyUP(Util::Keycode &key);*/
#endif
