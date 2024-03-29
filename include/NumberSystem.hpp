#ifndef BATTLECAT_NUMBERSYSTEM_HPP
#define BATTLECAT_NUMBERSYSTEM_HPP

#include "AnimatedGameObject.hpp"
class NumberSystem{
public:
    NumberSystem() = default;
    NumberSystem(std::initializer_list<std::string> &font);

    glm::vec2 Display(unsigned int number,glm::vec2 rightmost_pos,float zIndex);
    void SetNumberFont(std::initializer_list<std::string> &font);

private:

    short int GetCurrentDigit(unsigned int number,unsigned short int digit);
    short int GetDigits(unsigned int number);

    std::shared_ptr<AnimatedGameObject> m_num = std::make_shared<AnimatedGameObject>(std::initializer_list<std::string>
        {RESOURCE_DIR"/scene/0.png",RESOURCE_DIR"/scene/1.png",RESOURCE_DIR"/scene/2.png",RESOURCE_DIR"/scene/3.png",RESOURCE_DIR"/scene/4.png",
          RESOURCE_DIR"/scene/5.png",RESOURCE_DIR"/scene/6.png",RESOURCE_DIR"/scene/7.png",RESOURCE_DIR"/scene/8.png",RESOURCE_DIR"/scene/9.png"});

};

#endif // BATTLECAT_NUMBERSYSTEM_HPP
