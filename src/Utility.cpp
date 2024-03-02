#include "Utility.hpp"

bool PointInRect(const glm::vec2 rect_tl_pos, const glm::vec2 rect_size,
                 const glm::vec2 p) {
    return p.x >= rect_tl_pos.x && p.x <= rect_tl_pos.x + rect_size.x &&
           p.y >= rect_tl_pos.y && p.y <= rect_tl_pos.y + rect_size.y;
}
