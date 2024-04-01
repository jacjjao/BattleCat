#include "NumberSystem.hpp"

NumberSystem::NumberSystem(std::initializer_list<std::string> &font){
    SetNumberFont(font);
}

glm::vec2 NumberSystem::Display(unsigned const int number,const glm::vec2 rightmost_pos,const float zIndex) {
    auto X = rightmost_pos.x;
    auto Y = rightmost_pos.y;
    short digits = GetDigits(number);

    for(int i=0;i<digits;i++){
        short int digit = GetCurrentDigit(number,i);
        if(digit<10){
            m_num->Draw(Util::Transform{.translation=glm::vec2(X,Y)},zIndex,digit);
            X-=30;
        }
    }
    return glm::vec2(X+30,Y);
}

void NumberSystem::SetNumberFont(std::initializer_list<std::string> &font){
    m_num = std::make_shared<AnimatedGameObject>(font);
}

short int NumberSystem::GetCurrentDigit(unsigned int number,const unsigned short int digit) {
    for(short i=0;i<digit;i++) {
        number/= 10;
        if(number==0) { return 10;}
    }
    return static_cast<short>(number%10);
};

short int NumberSystem::GetDigits(unsigned int number){
    short int digits = 1;
    while(number > 0){
        number/=10;
        digits++;
    }
    return digits;
}
