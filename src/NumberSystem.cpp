#include "NumberSystem.hpp"

NumberSystem::NumberSystem(const std::initializer_list<std::string> font){
    SetNumberFont(font);
}

glm::vec2 NumberSystem::Display(unsigned int number, glm::vec2 rightmost_pos, const float zIndex,const float xOffset) {
    //constexpr int xOffset = 30;
    const auto drawDigit = [this, xOffset, &rightmost_pos](float z, unsigned int digit) {
        Util::Transform t;
        t.translation = rightmost_pos;
        m_num->Draw(t, z, digit);
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

void NumberSystem::SetNumberFont(const std::initializer_list<std::string> font){
    m_num = std::make_shared<AnimatedGameObject>(font);
}