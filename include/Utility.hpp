#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <glm/vec2.hpp>
#include "Util/Keycode.hpp"

[[nodiscard]] 
bool PointInRect(glm::vec2 rect_tl_pos, glm::vec2 rect_size, glm::vec2 p);
[[nodiscard]]
bool PosInRange(glm::vec2 LTop,glm::vec2 RBottom,glm::vec2 pos);

namespace Patch{
    extern bool mouse_last;
    extern bool mouse_current;
    void MouseUpdate();
    bool MouseLBUP();
}
#endif
