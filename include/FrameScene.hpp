#ifndef BATTLECAT_FRAMESCENE_HPP
#define BATTLECAT_FRAMESCENE_HPP

#include "Scene.hpp"
#include "GameObjectEx.hpp"
#include "Util/Image.hpp"

class FrameScene : public Scene{
public:
    void Setframes(const std::string &basetext) {
        m_frameL = std::make_shared<GameObjectEx>();
        m_frameR = std::make_shared<GameObjectEx>();
        m_frameUP = std::make_shared<GameObjectEx>();
        m_frameDOWN = std::make_shared<GameObjectEx>();
        m_BaseText = std::make_shared<GameObjectEx>();

        m_frameL->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/cat_base/frame_left.png"));
        m_frameR->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/cat_base/frame_right.png"));
        m_frameUP->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/cat_base/frame_up.png"));
        m_frameDOWN->SetDrawable(std::make_shared<Util::Image>(RESOURCE_DIR"/cat_base/frame_down.png"));
        m_BaseText->SetDrawable(std::make_shared<Util::Image>(basetext));

        m_frameL->SetZIndex(2);
        m_frameR->SetZIndex(2);
        m_frameUP->SetZIndex(1);
        m_frameDOWN->SetZIndex(1);
        m_BaseText->SetZIndex(1.1f);

        m_frameL->SetScale(1,2);
        m_frameR->SetScale(1,2);
        m_frameUP->SetScale(2,1);
        m_frameDOWN->SetScale(2,1);

        m_frameL->SetPosition(float(app_w)/-2.0f + 18,0);
        m_frameR->SetPosition(float(app_w)/2.0f - 18,0);
        m_frameUP->SetPosition(0,float(app_h)/2.0f - m_frameUP->GetScaledSize().y/2.0f);
        m_frameDOWN->SetPosition(0,float(app_h)/-2.0f + m_frameDOWN->GetScaledSize().y/2.0f);
        m_BaseText->SetPosition(float(app_w)/-2.0f + m_BaseText->GetScaledSize().x/2.0f + 60
                                ,float(app_h)/2.0f - m_BaseText->GetScaledSize().y/2.0f);

        m_Root.AddChild(m_frameL);
        m_Root.AddChild(m_frameR);
        m_Root.AddChild(m_frameUP);
        m_Root.AddChild(m_frameDOWN);
        m_Root.AddChild(m_BaseText);
    }
private:
    std::shared_ptr<GameObjectEx> m_frameL;
    std::shared_ptr<GameObjectEx> m_frameR;
    std::shared_ptr<GameObjectEx> m_frameUP;
    std::shared_ptr<GameObjectEx> m_frameDOWN;
    std::shared_ptr<GameObjectEx> m_BaseText;
};






















#endif // BATTLECAT_FRAMESCENE_HPP
