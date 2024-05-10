#ifndef STAGE_CARD_HPP
#define STAGE_CARD_HPP

#include <string>
#include "Util/Image.hpp"

class StageCard {
public:
    explicit StageCard(const std::string &titlepath);

    void Draw(float x, float y);

    void SetScale(float x, float y);

    glm::vec2 GetScaledSize() const;

    static glm::vec2 GetImgSize();

private:
    static inline const std::unique_ptr<Util::Image> s_CardImg = std::make_unique<Util::Image>(RESOURCE_DIR "/stages/stagecard.png");

    //std::string m_Title;
    Util::Image m_Title;

    glm::vec2 m_Scale;
};

#endif 