#include "StageCard.hpp"

StageCard::StageCard(std::string title)
    : m_Title(std::move(title)) {}

void StageCard::Draw(const Util::Transform &t) {
    s_CardImg->Draw(t, 10.0f);
}
