#include "Utility.hpp"
#include "Util/Input.hpp"

bool Patch::mouse_current = false;
bool Patch::mouse_last = false;

bool PointInRect(const glm::vec2 rect_tl_pos, const glm::vec2 rect_size,
                 const glm::vec2 p) {
    return p.x >= rect_tl_pos.x && p.x <= rect_tl_pos.x + rect_size.x &&
           p.y >= rect_tl_pos.y && p.y <= rect_tl_pos.y + rect_size.y;
}

bool PosInRange(const glm::vec2 LTop,const glm::vec2 RBottom,const glm::vec2 pos){
    return pos.x <= RBottom.x && LTop.x <= pos.x &&
           pos.y >= RBottom.y && LTop.y >= pos.y;
}

void Patch::MouseUpdate() {
    Patch::mouse_last = mouse_current;
    Patch::mouse_current = Util::Input::IsKeyPressed(Util::Keycode::MOUSE_LB);
}

bool Patch::MouseLBUP(){
    return !Patch::mouse_current && Patch::mouse_last;
}