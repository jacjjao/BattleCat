//
// Created by user on 2024/3/22.
//

#ifndef BATTLECAT_UNITCARD_HPP
#define BATTLECAT_UNITCARD_HPP

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
        std::stringstream ss;
        ss << RESOURCE_DIR"/cats/udi/" << "udi" << std::string(UnitNumLength - std::to_string(unitnum).length(), '0') << unitnum << "_f.png";
        //printf("%s/n",ss.str().c_str());
        m_udi1 = std::make_shared<GameObjectEx>(std::make_unique<Util::Image>(ss.str()),zIndex+0.001f);
        m_udi1->SetPosition(-2.0f,32.0f);
        AddChild(m_udi1);
    };

    void MinifyAnime();

    void AmplifyAnime();

    void Dragging() override;

    void Put_OFF() override;

    void SetVisible(bool b);

    //void SetScale(float x, float y);

    bool IsMouseHovering() override;

    unsigned int GetUnitNum(){ return m_UnitNum;};

private:
    std::shared_ptr<GameObjectEx> m_udi1;
    std::shared_ptr<GameObjectEx> m_udi2;
    short m_FrameTimer = 0;
    Util::Transform m_DragTrans;
    bool m_minify = false;
    unsigned int m_UnitNum = 0;
};

#endif // BATTLECAT_UNITCARD_HPP
