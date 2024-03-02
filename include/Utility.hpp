#pragma once

#include <glm/vec2.hpp>

[[nodiscard]] 
bool PointInRect(glm::vec2 rect_tl_pos, glm::vec2 rect_size, glm::vec2 p);
