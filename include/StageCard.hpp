#ifndef STAGE_CARD_HPP
#define STAGE_CARD_HPP

#include <string>
#include "Util/Image.hpp"

class StageCard {
public:
    explicit StageCard(std::string title);

    void Draw(const Util::Transform &t);

private:
    static inline const std::unique_ptr<Util::Image> s_CardImg = std::make_unique<Util::Image>(RESOURCE_DIR "/stages/stagecard.png");

    std::string m_Title;
};

#endif 