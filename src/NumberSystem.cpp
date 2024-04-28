#include "NumberSystem.hpp"
#include "Util/Transform.hpp"

std::array<Util::Image,10> NumberSystem::DarkBit =
    {Util::Image(RESOURCE_DIR"/scene/darkbit/0.png"),Util::Image(RESOURCE_DIR"/scene/darkbit/1.png"),Util::Image(RESOURCE_DIR"/scene/darkbit/2.png"),Util::Image(RESOURCE_DIR"/scene/darkbit/3.png"),Util::Image(RESOURCE_DIR"/scene/darkbit/4.png"),
     Util::Image(RESOURCE_DIR"/scene/darkbit/5.png"),Util::Image(RESOURCE_DIR"/scene/darkbit/6.png"),Util::Image(RESOURCE_DIR"/scene/darkbit/7.png"),Util::Image(RESOURCE_DIR"/scene/darkbit/8.png"),Util::Image(RESOURCE_DIR"/scene/darkbit/9.png")};

std::array<Util::Image,10> NumberSystem::DarkNumber =
    {Util::Image(RESOURCE_DIR"/scene/darknumber/0.png"),Util::Image(RESOURCE_DIR"/scene/darknumber/1.png"),Util::Image(RESOURCE_DIR"/scene/darknumber/2.png"),Util::Image(RESOURCE_DIR"/scene/darknumber/3.png"),Util::Image(RESOURCE_DIR"/scene/darknumber/4.png"),
     Util::Image(RESOURCE_DIR"/scene/darknumber/5.png"),Util::Image(RESOURCE_DIR"/scene/darknumber/6.png"),Util::Image(RESOURCE_DIR"/scene/darknumber/7.png"),Util::Image(RESOURCE_DIR"/scene/darknumber/8.png"),Util::Image(RESOURCE_DIR"/scene/darknumber/9.png")};

std::array<Util::Image,10> NumberSystem::SmallNumber =
    {Util::Image(RESOURCE_DIR"/scene/smallnumber/0.png"),Util::Image(RESOURCE_DIR"/scene/smallnumber/1.png"),Util::Image(RESOURCE_DIR"/scene/smallnumber/2.png"),Util::Image(RESOURCE_DIR"/scene/smallnumber/3.png"),Util::Image(RESOURCE_DIR"/scene/smallnumber/4.png"),
     Util::Image(RESOURCE_DIR"/scene/smallnumber/5.png"),Util::Image(RESOURCE_DIR"/scene/smallnumber/6.png"),Util::Image(RESOURCE_DIR"/scene/smallnumber/7.png"),Util::Image(RESOURCE_DIR"/scene/smallnumber/8.png"),Util::Image(RESOURCE_DIR"/scene/smallnumber/9.png")};

std::array<Util::Image,10> NumberSystem::WhiteNumber =
    {Util::Image(RESOURCE_DIR"/scene/whitenumber/0.png"),Util::Image(RESOURCE_DIR"/scene/whitenumber/1.png"),Util::Image(RESOURCE_DIR"/scene/whitenumber/2.png"),Util::Image(RESOURCE_DIR"/scene/whitenumber/3.png"),Util::Image(RESOURCE_DIR"/scene/whitenumber/4.png"),
     Util::Image(RESOURCE_DIR"/scene/whitenumber/5.png"),Util::Image(RESOURCE_DIR"/scene/whitenumber/6.png"),Util::Image(RESOURCE_DIR"/scene/whitenumber/7.png"),Util::Image(RESOURCE_DIR"/scene/whitenumber/8.png"),Util::Image(RESOURCE_DIR"/scene/whitenumber/9.png")};

std::array<Util::Image,10> NumberSystem::YellowBit =
    {Util::Image(RESOURCE_DIR"/scene/yellowbit/0.png"),Util::Image(RESOURCE_DIR"/scene/yellowbit/1.png"),Util::Image(RESOURCE_DIR"/scene/yellowbit/2.png"),Util::Image(RESOURCE_DIR"/scene/yellowbit/3.png"),Util::Image(RESOURCE_DIR"/scene/yellowbit/4.png"),
     Util::Image(RESOURCE_DIR"/scene/yellowbit/5.png"),Util::Image(RESOURCE_DIR"/scene/yellowbit/6.png"),Util::Image(RESOURCE_DIR"/scene/yellowbit/7.png"),Util::Image(RESOURCE_DIR"/scene/yellowbit/8.png"),Util::Image(RESOURCE_DIR"/scene/yellowbit/9.png")};

std::array<Util::Image,10> NumberSystem::YellowNumber =
    {Util::Image(RESOURCE_DIR"/scene/yellownumber/0.png"),Util::Image(RESOURCE_DIR"/scene/yellownumber/1.png"),Util::Image(RESOURCE_DIR"/scene/yellownumber/2.png"),Util::Image(RESOURCE_DIR"/scene/yellownumber/3.png"),Util::Image(RESOURCE_DIR"/scene/yellownumber/4.png"),
     Util::Image(RESOURCE_DIR"/scene/yellownumber/5.png"),Util::Image(RESOURCE_DIR"/scene/yellownumber/6.png"),Util::Image(RESOURCE_DIR"/scene/yellownumber/7.png"),Util::Image(RESOURCE_DIR"/scene/yellownumber/8.png"),Util::Image(RESOURCE_DIR"/scene/yellownumber/9.png")};

glm::vec2 NumberSystem::Display(unsigned int number, glm::vec2 rightmost_pos, const float zIndex,const float xOffset,std::array<Util::Image,10>& font) {
    //constexpr int xOffset = 30;
    const auto drawDigit = [&font, xOffset, &rightmost_pos](float z, unsigned int digit) {
        Util::Transform t;
        t.translation = rightmost_pos;
        font[digit].Draw(t, z);
        rightmost_pos.x -= xOffset;
    };

    if (number == 0) {
        drawDigit(zIndex, 0);
        return rightmost_pos;
    }

    for (; number > 0; number /= 10) {
        drawDigit(zIndex, number % 10);
    }
    glm::vec2 result = glm::vec2(rightmost_pos.x + xOffset, rightmost_pos.y);
    return result;
}
