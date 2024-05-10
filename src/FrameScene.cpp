#include "FrameScene.hpp"

void FrameScene::SetBaseText(const std::string &basetext) {
    m_BaseText = std::make_shared<GameObjectEx>();
    m_BaseText->SetDrawable(std::make_shared<Util::Image>(basetext));
    m_BaseText->SetZIndex(1.1f);
    m_BaseText->SetPosition(float(app_w)/-2.0f + m_BaseText->GetScaledSize().x/2.0f + 60
                            ,float(app_h)/2.0f - m_BaseText->GetScaledSize().y/2.0f);
    m_Root.AddChild(m_BaseText);
}

void FrameScene::AddXP(const unsigned int xp) {
    m_XP += xp;
    if(m_XP > 99999999){
        m_XP = 99999999;
    }
}

void FrameScene::CostXP(const unsigned int xp) {
    if (m_XP >= xp) {
        m_XP -= xp;
    }
}

void FrameScene::AddCatfood(unsigned int Catfood) {
    m_Catfood += Catfood;
    if(m_Catfood > 9999999){
        m_Catfood = 9999999;
    }
}

void FrameScene::CostCatfood(unsigned int Catfood) {
    if (m_Catfood >= Catfood) {
        m_Catfood -= Catfood;
    }
}

void FrameScene::Update() {
    NumberSystem::Display(m_XP,glm::vec2(float(app_w)/2.0f - 72,float(app_h)/2.0f - 26.5),2.0f,30,NumberSystem::YellowNumber);
    NumberSystem::Display(m_Catfood,glm::vec2(float(app_w)/2.0f - 72,float(app_h)/-2.0f + 26.5),2.0f,30,NumberSystem::YellowNumber);
}
