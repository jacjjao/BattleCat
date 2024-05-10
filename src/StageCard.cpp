#include "StageCard.hpp"

StageCard::StageCard(const std::string &titlepath) : m_Title(titlepath)
      ,m_Scale(1.0f, 1.0f){}

void StageCard::Draw(const float x, const float y) {
    Util::Transform t;
    t.scale = m_Scale;
    t.translation = glm::vec2(x, y);
    s_CardImg->Draw(t, 1.8f);
    m_Title.Draw(t,1.9f);
}

void StageCard::SetScale(const float x, const float y) {
    m_Scale = glm::vec2(x, y);
}

glm::vec2 StageCard::GetScaledSize() const {
    return s_CardImg->GetSize() * m_Scale;
}

glm::vec2 StageCard::GetImgSize() {
    return s_CardImg->GetSize();
}
