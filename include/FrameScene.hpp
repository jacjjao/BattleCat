#ifndef BATTLECAT_FRAMESCENE_HPP
#define BATTLECAT_FRAMESCENE_HPP

#include "Scene.hpp"
#include "GameObjectEx.hpp"
#include "Util/Image.hpp"
#include "NumberSystem.hpp"

class FrameScene : public Scene{
public:
    FrameScene(){
        m_Root.AddChild(m_frameL);
        m_Root.AddChild(m_frameR);
        m_Root.AddChild(m_frameUP);
        m_Root.AddChild(m_frameDOWN);
        m_Root.AddChild(m_XPimg);
        m_Root.AddChild(m_Catfoodimg);
        m_Root.AddChild(m_XPframe);
        m_Root.AddChild(m_Catfoodframe);
    }
    void SetBaseText(const std::string &basetext);

    static unsigned int GetXP(){ return m_XP; };
    static void AddXP(unsigned int xp);

    static unsigned int GetCatfood(){ return m_Catfood; };
    static void AddCatfood(unsigned int Catfood);

    virtual void Update() override;

    virtual ~FrameScene() override = default;

private:
    static std::shared_ptr<GameObjectEx> Init
        (const std::string &path, const float &z, const glm::vec2 &scale, const glm::vec2 &pos){
        auto frame = std::make_shared<GameObjectEx>();
        frame->SetDrawable(std::make_shared<Util::Image>(path));
        frame->SetZIndex(z);
        frame->SetScale(scale);
        frame->SetPosition(pos);
        return frame;
    };

    //-----------------------------------------------------------------------------------
    static inline const std::shared_ptr<GameObjectEx> m_frameL = Init
        (RESOURCE_DIR"/cat_base/frame_left.png",2,glm::vec2(1,2),
         glm::vec2(float(app_w)/-2.0f + 18,0));

    static inline const std::shared_ptr<GameObjectEx> m_frameR = Init
        (RESOURCE_DIR"/cat_base/frame_right.png",2,glm::vec2(1,2),
         glm::vec2(float(app_w)/2.0f - 18,0));

    static inline const std::shared_ptr<GameObjectEx> m_frameUP = Init
        (RESOURCE_DIR"/cat_base/frame_up.png",1,glm::vec2(2,1),
         glm::vec2(0,float(app_h)/2.0f - 27.5));

    static inline const std::shared_ptr<GameObjectEx> m_frameDOWN = Init
        (RESOURCE_DIR"/cat_base/frame_down.png",1,glm::vec2(2,1),
         glm::vec2(0,float(app_h)/-2.0f + 27.5));

    static inline const  std::shared_ptr<GameObjectEx> m_XPframe = Init
        (RESOURCE_DIR"/cat_base/numberframe.png",1.01f,glm::vec2(1,1),
         glm::vec2(float(app_w)/2.9f,float(app_h)/2.0f - 28.5));

    static inline const  std::shared_ptr<GameObjectEx> m_Catfoodframe = Init
        (RESOURCE_DIR"/cat_base/numberframe.png",1.01f,glm::vec2(1,-1),
         glm::vec2(float(app_w)/2.9f,float(app_h)/-2.0f + 28.5));

    static inline const  std::shared_ptr<GameObjectEx> m_XPimg = Init
        (RESOURCE_DIR"/cat_base/XP.png",1.2f,glm::vec2(0.7f,0.7f),
         glm::vec2(float(app_w)/4.5f,float(app_h)/2.0f - 27.5));

    static inline const  std::shared_ptr<GameObjectEx> m_Catfoodimg = Init
        (RESOURCE_DIR"/cat_base/catfood.png",1.2f,glm::vec2(0.7f,0.7f),
         glm::vec2(float(app_w)/3.8f,float(app_h)/-2.0f + 27.5));

    std::shared_ptr<GameObjectEx> m_BaseText;
    NumberSystem m_NumSys;

protected:
    static inline unsigned int m_XP = 99999999;
    static inline unsigned int m_Catfood = 9999999;
};

#endif // BATTLECAT_FRAMESCENE_HPP
