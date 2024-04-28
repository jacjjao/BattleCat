#ifndef BATTLECAT_NUMBERSYSTEM_HPP
#define BATTLECAT_NUMBERSYSTEM_HPP

#include "AnimatedGameObject.hpp"
class NumberSystem{
public:
    NumberSystem() = default;
    NumberSystem(std::initializer_list<std::string> font);

    glm::vec2 Display(unsigned int number,glm::vec2 rightmost_pos,float zIndex,float xOffset);
    void SetNumberFont(std::initializer_list<std::string> font);

private:
    std::shared_ptr<AnimatedGameObject> m_num = std::make_shared<AnimatedGameObject>(std::initializer_list<std::string>
        {RESOURCE_DIR"/scene/yellownumber/0.png",RESOURCE_DIR"/scene/yellownumber/1.png",RESOURCE_DIR"/scene/yellownumber/2.png",RESOURCE_DIR"/scene/yellownumber/3.png",RESOURCE_DIR"/scene/yellownumber/4.png",
          RESOURCE_DIR"/scene/yellownumber/5.png",RESOURCE_DIR"/scene/yellownumber/6.png",RESOURCE_DIR"/scene/yellownumber/7.png",RESOURCE_DIR"/scene/yellownumber/8.png",RESOURCE_DIR"/scene/yellownumber/9.png"});

};

#endif // BATTLECAT_NUMBERSYSTEM_HPP
