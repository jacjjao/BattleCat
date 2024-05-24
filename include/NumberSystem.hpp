#ifndef BATTLECAT_NUMBERSYSTEM_HPP
#define BATTLECAT_NUMBERSYSTEM_HPP

#include <array>
#include <glm/vec2.hpp>
#include "Util/Image.hpp"

namespace NumberSystem{
    extern std::array<Util::Image,10> DarkBit;

    extern std::array<Util::Image,10> DarkNumber;

    extern std::array<Util::Image,10> SmallNumber;

    extern std::array<Util::Image,10> WhiteNumber;

    extern std::array<Util::Image,10> YellowBit;

    extern std::array<Util::Image,10> YellowNumber;

    glm::vec2 Display(unsigned int number,glm::vec2 rightmost_pos,float zIndex,float xOffset,std::array<Util::Image,10>& font,glm::vec2 scale = {1,1});
}

#endif // BATTLECAT_NUMBERSYSTEM_HPP
