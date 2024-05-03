#include "StageCard.hpp"

StageCard::StageCard(std::string title)
    : m_Title(std::move(title)),
      m_Scale(1.0f, 1.0f) {}

void StageCard::Draw(const float x, const float y) {
    Util::Transform t;
    t.scale = m_Scale;
    t.translation = glm::vec2(x, y);
    s_CardImg->Draw(t, 10.0f);
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
