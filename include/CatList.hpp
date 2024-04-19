#ifndef BATTLECAT_CATLIST_HPP
#define BATTLECAT_CATLIST_HPP

#include "App.hpp"
#include "GameObjectEx.hpp"
#include "Util/Image.hpp"
#include "Draggable.hpp"
#include "GameButton.hpp"
#include <sstream>
#include <iostream>

class UnitCard : public Draggable , public GameObjectEx{
public:
    UnitCard(unsigned int unitnum,const float zIndex){
        const int UnitNumLength = 3;
        SetDrawable(std::make_unique<Util::Image>(RESOURCE_DIR"/cats/unit.png"));
        SetZIndex(zIndex);
        m_DragTrans.scale = glm::vec2(1.4f, 1.4f);

        m_UnitNum = unitnum;
        std::stringstream udi1_img;
        udi1_img << RESOURCE_DIR"/cats/udi/" << "udi" << std::string(UnitNumLength - std::to_string(unitnum).length(), '0') << unitnum<<"_f.png";
        m_udi1 = std::make_shared<GameObjectEx>(std::make_unique<Util::Image>(udi1_img.str()),zIndex+0.001f);
        m_udi1->SetPosition(-2.0f,32.0f);
        AddChild(m_udi1);

        std::stringstream udi2_img;
        udi2_img << RESOURCE_DIR"/cats/udi/" << "udi" << std::string(UnitNumLength - std::to_string(unitnum).length(), '0') << unitnum << "_c.png";
        m_udi2 = std::make_shared<GameObjectEx>(std::make_unique<Util::Image>(udi2_img.str()),zIndex+0.001f);
        m_udi2->SetPosition(-2.0f,32.0f);
        AddChild(m_udi2);

        m_udi2->SetVisible(false);

        m_currudi = m_udi1;
    };
    void Transform();

    void MinifyAnime();

    void AmplifyAnime();

    void SetVisible(bool b);

    //void SetScale(float x, float y);
    unsigned int GetUnitNum(){ return m_UnitNum;};

    bool Getform(){return m_form;};
    ;
private:
    //void Unpressed() override;
    void Dragging() override;
    void Put_OFF() override;
    //void PickUp() override;
    bool IsMouseHovering() override;

private:
    std::shared_ptr<GameObjectEx> m_udi1;
    std::shared_ptr<GameObjectEx> m_udi2;
    std::shared_ptr<GameObjectEx> m_currudi;
    short m_FrameTimer = 0;
    Util::Transform m_DragTrans;
    bool m_minify = false;
    unsigned int m_UnitNum = 0;
    bool m_form = false;
};
//--------------------------------------------------------------------------------------
class CatList{
private:
    CatList() = delete;
    static std::vector<std::shared_ptr<UnitCard>> Init(){
        std::vector<std::shared_ptr<UnitCard>> catlist;
        catlist.reserve(MAXUNITS);
        for(unsigned int i=0;i<MAXUNITS;i++){
            auto &unit = catlist.emplace_back
                         (std::make_shared<UnitCard>(i,1.9f));
            //m_Root. AddChild(unit);
        }
        return catlist;
    }
    static inline std::vector<std::shared_ptr<UnitCard>> m_catlist = Init();
public:
    static std::vector<std::shared_ptr<UnitCard>> GetCatList(){return m_catlist;};
};

#endif // BATTLECAT_CATLIST_HPP
