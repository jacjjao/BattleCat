#ifndef BATTLECAT_FRAMESCENE_HPP
#define BATTLECAT_FRAMESCENE_HPP

#include "Scene.hpp"
#include "GameObjectEx.hpp"
#include "Util/Image.hpp"

class FrameScene : public Scene{
public:
    FrameScene(){
        m_Root.AddChild(m_frameL);
        m_Root.AddChild(m_frameR);
        m_Root.AddChild(m_frameUP);
        m_Root.AddChild(m_frameDOWN);
    }
    void SetBaseText(const std::string &basetext){
        m_BaseText = std::make_shared<GameObjectEx>();
        m_BaseText->SetDrawable(std::make_shared<Util::Image>(basetext));
        m_BaseText->SetZIndex(1.1f);
        m_BaseText->SetPosition(float(app_w)/-2.0f + m_BaseText->GetScaledSize().x/2.0f + 60
                                ,float(app_h)/2.0f - m_BaseText->GetScaledSize().y/2.0f);
        m_Root.AddChild(m_BaseText);
    }
private:
    static std::shared_ptr<GameObjectEx> Set
        (const std::string &path, const float &z, const glm::vec2 &scale, const glm::vec2 &pos){
        auto frame = std::make_shared<GameObjectEx>();
        frame->SetDrawable(std::make_shared<Util::Image>(path));
        frame->SetZIndex(z);
        frame->SetScale(scale);
        frame->SetPosition(pos);
        return frame;
    };

    //-----------------------------------------------------------------------------------
    static inline const std::shared_ptr<GameObjectEx> m_frameL = Set
        (RESOURCE_DIR"/cat_base/frame_left.png",2,glm::vec2(1,2),
         glm::vec2(float(app_w)/-2.0f + 18,0));

    static inline const std::shared_ptr<GameObjectEx> m_frameR = Set
        (RESOURCE_DIR"/cat_base/frame_right.png",2,glm::vec2(1,2),
         glm::vec2(float(app_w)/2.0f - 18,0));

    static inline const std::shared_ptr<GameObjectEx> m_frameUP = Set
        (RESOURCE_DIR"/cat_base/frame_up.png",1,glm::vec2(2,1),
         glm::vec2(0,float(app_h)/2.0f - 27.5));

    static inline const std::shared_ptr<GameObjectEx> m_frameDOWN =Set
        (RESOURCE_DIR"/cat_base/frame_down.png",1,glm::vec2(2,1),
         glm::vec2(0,float(app_h)/-2.0f + 27.5));

    std::shared_ptr<GameObjectEx> m_BaseText;

};

#endif // BATTLECAT_FRAMESCENE_HPP
